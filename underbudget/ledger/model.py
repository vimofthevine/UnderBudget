# UnderBudget
# Copyright 2018 Kyle Treubig
#
# UnderBudget is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# UnderBudget is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with UnderBudget.  If not, see <http://www.gnu.org/licenses/>.


from money import Money as MoneyBase

from sqlalchemy import Boolean, Date
from sqlalchemy import Column
from sqlalchemy import ForeignKey
from sqlalchemy import Integer
from sqlalchemy import select
from sqlalchemy import String
from sqlalchemy import and_
from sqlalchemy.ext.declarative import declarative_base
from sqlalchemy.orm import aliased, composite
from sqlalchemy.orm import backref
from sqlalchemy.orm import relationship
from sqlalchemy.orm.exc import NoResultFound
from sqlalchemy.sql.expression import func

from underbudget.db import Base


class Currency(Base):
    """Model of a single monetary currency."""

    __tablename__ = 'currency'

    id = Column(Integer, primary_key=True)
    code = Column(String)
    external_id = Column('ext_id', String)

    def __init__(self, **kwargs):
        self.id = 1
        self.code = 'USD'
        super().__init__(**kwargs)


class Money(MoneyBase):
    """A discrete amount of money in a specific currency."""

    def __init__(self, amount=0.0, currency=Currency()):
        """Initialize the money.

        Keyword arguments:
        amount   -- money amount
        currency -- Currency object instance
        """
        super().__init__(amount=amount, currency=currency.code)
        self._currency_obj = currency


class Account(Base):
    """Model of cash on hand or a real-world bank, credit card, or loan account."""

    __tablename__ = 'account'

    id = Column(Integer, primary_key=True)
    name = Column(String)
    currency_id = Column(Integer, ForeignKey('currency.id', ondelete='SET DEFAULT'),
                         default=1)
    currency = relationship('Currency', foreign_keys='[Account.currency_id]')
    archived = Column(Boolean, default=False)
    external_id = Column('ext_id', String)
    parent_id = Column(Integer, ForeignKey('account.id'), default=1)
    children = relationship('Account', cascade='all, delete, delete-orphan',
                            backref=backref('parent', remote_side=[id]))

    def __repr__(self):
        return '{0} ({1})'.format(self.name, self.id)


class Envelope(Base):
    """Model of a designated portion of available funds."""

    __tablename__ = 'envelope'

    id = Column(Integer, primary_key=True)
    name = Column(String)
    currency_id = Column(Integer, ForeignKey('currency.id', ondelete='SET DEFAULT'),
                         default=1)
    currency = relationship('Currency', foreign_keys='[Envelope.currency_id]')
    archived = Column(Boolean, default=False)
    external_id = Column('ext_id', String)
    parent_id = Column(Integer, ForeignKey('envelope.id'), default=1)
    children = relationship('Envelope', cascade='all, delete, delete-orphan',
                            backref=backref('parent', remote_side=[id]))

    def __repr__(self):
        return '{0} ({1})'.format(self.name, self.id)


class Reconciliation(Base):
    """Model of an account reconciliation against a statement"""

    __tablename__ = 'reconciliation'

    id = Column(Integer, primary_key=True)
    account_id = Column(Integer, ForeignKey('account.id', ondelete='CASCADE'))
    account = relationship('Account')
    _beginning_balance = Column('beginning_balance', Integer)
    beginning_date = Column(Date)
    _ending_balance = Column('ending_balance', Integer)
    ending_date = Column(Date)

    @property
    def beginning_balance(self):
        return Money(self._beginning_balance, self.account.currency)

    @beginning_balance.setter
    def beginning_balance(self, value):
        if type(value) in [Money, MoneyBase]:
            self._beginning_balance = value.amount
        else:
            self._beginning_balance = int(value)

    @property
    def ending_balance(self):
        return Money(self._ending_balance, self.account.currency)

    @ending_balance.setter
    def ending_balance(self, value):
        if type(value) in [Money, MoneyBase]:
            self._ending_balance = value.amount
        else:
            self._ending_balance = int(value)


class Transaction(Base):
    """Model of a double-entry transaction against accounts and envelopes."""

    __tablename__ = 'transaction_entry'

    id = Column(Integer, primary_key=True)
    date = Column(Date)
    payee = Column(String)
    account_transactions = relationship('AccountTransaction', back_populates='transaction')
    envelope_transactions = relationship('EnvelopeTransaction', back_populates='transaction')
    balance = Money()

    def __repr__(self):
        return '{0} {1} ({2})'.format(self.date, self.payee, self.id)


class AccountTransaction(Base):
    """Model of a transaction against a single account as part of a double-entry transaction."""

    __tablename__ = 'account_transaction'

    id = Column(Integer, primary_key=True)
    transaction_entry_id = Column(Integer, ForeignKey('transaction_entry.id', ondelete='CASCADE'))
    transaction = relationship('Transaction', back_populates='account_transactions')
    account_id = Column(Integer, ForeignKey('account.id', ondelete='RESTRICT'))
    account = relationship('Account')
    _amount = Column('amount', Integer)
    memo = Column(String)
    cleared = Column(Boolean)
    reconciliation_id = Column(Integer, ForeignKey('reconciliation.id', ondelete='SET NULL'))
    reconciliation = relationship('Reconciliation')

    @property
    def amount(self):
        return Money(self._amount, self.account.currency)

    @amount.setter
    def amount(self, value):
        if type(value) in [Money, MoneyBase]:
            self._amount = value.amount
        else:
            self._amount = int(value)


class EnvelopeTransaction(Base):
    """Model of a transaction against a single envelope as part of a double-entry transaction."""

    __tablename__ = 'envelope_transaction'

    id = Column(Integer, primary_key=True)
    transaction_entry_id = Column(Integer, ForeignKey('transaction_entry.id', ondelete='CASCADE'))
    transaction = relationship('Transaction', back_populates='envelope_transactions')
    envelope_id = Column(Integer, ForeignKey('envelope.id', ondelete='RESTRICT'))
    envelope = relationship('Envelope')
    _amount = Column('amount', Integer)
    memo = Column(String)

    @property
    def amount(self):
        return Money(self._amount, self.account.currency)

    @amount.setter
    def amount(self, value):
        if type(value) in [Money, MoneyBase]:
            self._amount = value.amount
        else:
            self._amount = int(value)


class JournalEntry(object):
    """A journal entry with account and envelope transactions.

    The journal entry ensures that the transaction has all required splits (account or
    envelope transactions) and has a zero-sum. That is, the sum of all account transactions
    less the sum of all envelope transactions is zero.
    """

    def __init__(self, transactions, transaction=None, copy=False):
        self._transactions = transactions


def init(session):
    """Adds root rows to all required tables"""
    if not session.query(Currency).filter(Currency.id == 1).one_or_none():
        usd = Currency(id=1, code='USD')
        session.add(usd)
    if not session.query(Account).filter(Account.id == 1).one_or_none():
        root = Account(id=1, name='root', parent_id=0)
        session.add(root)
    if not session.query(Envelope).filter(Envelope.id == 1).one_or_none():
        root = Envelope(id=1, name='root', parent_id=0)
        session.add(root)


def get_account(session, id):
    """Retrieves the requested account"""
    return session.query(Account).filter(Account.id == id).one()


def get_leaf_accounts(session):
    """Retrieves all leaf accounts"""
    # Perform a LEFT JOIN where the right side is the accounts with a parent
    # matching the left side, then return only the results where there is no
    # valid right side (i.e., no children)
    right = aliased(Account)
    return session.query(Account) \
        .join(right, right.parent_id == Account.id, isouter=True) \
        .filter(right.id.is_(None)).all()


def get_root_account(session):
    """Retrieves the root account"""
    return get_account(session, 1)


def get_envelope(session, id):
    """Retrieves the requested envelope"""
    return session.query(Envelope).filter(Envelope.id == id).one()


def get_leaf_envelopes(session):
    """Retrieves all leaf envelopes"""
    # Perform a LEFT JOIN where the right side is the envelopes with a parent
    # matching the left side, then return only the results where there is no
    # valid right side (i.e., no children)
    right = aliased(Envelope)
    return session.query(Envelope) \
        .join(right, right.parent_id == Envelope.id, isouter=True) \
        .filter(right.id.is_(None)).all()


def get_root_envelope(session):
    """Retrieves the root envelope"""
    return get_envelope(session, 1)


def get_account_transaction(session, id):
    """Retrieves the requested account transaction"""
    trn = session.query(AccountTransaction).filter(AccountTransaction.id == id).one()
    # TODO set balance
    return trn


def get_balance(session, date, currency=Currency(), account=None, envelope=None, cleared=None):
    """Calculates the total balance on the given date"""
    if account:
        query = session.query(func.sum(AccountTransaction._amount)) \
            .join(AccountTransaction.transaction) \
            .filter(and_(Transaction.date <= date, AccountTransaction.account == account))
        if cleared is not None:
            query = query.filter(AccountTransaction.cleared == cleared)
        currency = account.currency
    elif envelope:
        query = session.query(func.sum(EnvelopeTransaction._amount)) \
            .join(EnvelopeTransaction.transaction) \
            .filter(and_(Transaction.date <= date, EnvelopeTransaction.envelope == envelope))
        currency = envelope.currency
    else:
        query = session.query(func.sum(AccountTransaction._amount)) \
            .join(AccountTransaction.transaction).join(AccountTransaction.account) \
            .filter(and_(Transaction.date <= date, Account.currency == currency))
        if cleared is not None:
            query = query.filter(AccountTransaction.cleared == cleared)
    return Money(amount=query.scalar(), currency=currency)


