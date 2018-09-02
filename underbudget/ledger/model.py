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


from money import Money

from sqlalchemy import Boolean
from sqlalchemy import Column
from sqlalchemy import Date
from sqlalchemy import ForeignKey
from sqlalchemy import Integer
from sqlalchemy import String
from sqlalchemy import and_
from sqlalchemy.orm import aliased
from sqlalchemy.orm import backref
from sqlalchemy.orm import relationship
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


class Account(Base):
    """Model of cash on hand or a real-world bank, credit card, or loan account."""

    __tablename__ = 'account'

    id = Column(Integer, primary_key=True)
    name = Column(String, nullable=False)
    currency_id = Column(Integer, ForeignKey('currency.id', ondelete='SET DEFAULT'),
                         server_default='1')
    currency = relationship('Currency', foreign_keys='[Account.currency_id]')
    archived = Column(Boolean, default=False, nullable=False)
    external_id = Column('ext_id', String)
    parent_id = Column(Integer, ForeignKey('account.id'))
    children = relationship('Account', cascade='all, delete, delete-orphan',
                            backref=backref('parent', remote_side=[id]))

    def __repr__(self):
        return '{0} ({1})'.format(self.name, self.id)


class Envelope(Base):
    """Model of a designated portion of available funds."""

    __tablename__ = 'envelope'

    id = Column(Integer, primary_key=True)
    name = Column(String, nullable=False)
    currency_id = Column(Integer, ForeignKey('currency.id', ondelete='SET DEFAULT'),
                         server_default='1')
    currency = relationship('Currency', foreign_keys='[Envelope.currency_id]')
    archived = Column(Boolean, default=False, nullable=False)
    external_id = Column('ext_id', String)
    parent_id = Column(Integer, ForeignKey('envelope.id'))
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
        return create_money(self._beginning_balance, self.account.currency)

    @beginning_balance.setter
    def beginning_balance(self, value):
        self._beginning_balance = get_scaled(value)

    @property
    def ending_balance(self):
        return create_money(self._ending_balance, self.account.currency)

    @ending_balance.setter
    def ending_balance(self, value):
        self._ending_balance = get_scaled(value)


class Transaction(Base):
    """Model of a double-entry transaction against accounts and envelopes."""

    __tablename__ = 'transaction_entry'

    id = Column(Integer, primary_key=True)
    date = Column(Date)
    payee = Column(String)
    account_transactions = relationship('AccountTransaction', back_populates='transaction')
    envelope_transactions = relationship('EnvelopeTransaction', back_populates='transaction')

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

    def __init__(self, **kwargs):
        self._amount = 0
        super().__init__(**kwargs)

    @property
    def amount(self):
        return create_money(self._amount, self.account.currency)

    @amount.setter
    def amount(self, value):
        self._amount = get_scaled(value)


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

    def __init__(self, **kwargs):
        self._amount = 0
        super().__init__(**kwargs)

    @property
    def amount(self):
        return create_money(self._amount, self.envelope.currency)

    @amount.setter
    def amount(self, value):
        self._amount = get_scaled(value)


def init(session):
    """Adds root rows to all required tables"""
    if not session.query(Currency).filter(Currency.id == 1).one_or_none():
        usd = Currency(id=1, code='USD')
        session.add(usd)
    if not session.query(Account).filter(Account.id == 1).one_or_none():
        root = Account(id=1, name='root')
        session.add(root)
    if not session.query(Envelope).filter(Envelope.id == 1).one_or_none():
        root = Envelope(id=1, name='root')
        session.add(root)


def create_money(scaled, currency):
    """Creates a money object from the scaled value"""
    if type(currency) is Currency:
        code = currency.code
    else:
        code = currency
    if scaled is None:
        return Money(amount=0, currency=code)
    else:
        return Money(amount=float(scaled) / 10000.0, currency=code)


def get_scaled(value):
    """Extracts the scalar value from the given money object"""
    if type(value) is Money:
        return int(value.amount * 10000)
    else:
        return int(value * 10000)


def get_account(session, id):
    """Retrieves the requested account"""
    return session.query(Account).filter(Account.id == id).one_or_none()


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
    return session.query(Envelope).filter(Envelope.id == id).one_or_none()


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


def copy(transaction):
    """Creates a copy of the given transaction"""
    copy_trn = Transaction(date=transaction.date, payee=transaction.payee)
    for trn in transaction.account_transactions:
        copy_trn.account_transactions.append(
            AccountTransaction(account=trn.account, amount=trn.amount, cleared=trn.cleared,
                               memo=trn.memo)
        )
    for trn in transaction.envelope_transactions:
        copy_trn.envelope_transactions.append(
            EnvelopeTransaction(envelope=trn.envelope, amount=trn.amount, memo=trn.memo)
        )
    return copy_trn


def validate(transaction):
    """Verifies if the given transaction is a valid double-entry transaction"""
    num_acct_splits = len(transaction.account_transactions)
    num_env_splits = len(transaction.envelope_transactions)

    if num_acct_splits == 0 and num_env_splits == 0:
        return "No account or envelope splits defined"

    if num_acct_splits > 1 and num_env_splits > 1:
        return "Multiple account and multiple envelope splits defined"

    if num_acct_splits == 0:
        currency = transaction.envelope_transactions[0].envelope.currency
    else:
        currency = transaction.account_transactions[0].account.currency
    acct_total = create_money(0, currency)
    env_total = create_money(0, currency)

    for trn in transaction.account_transactions:
        if currency != trn.account.currency:
            return "Currency conversion would be required but is not supported"
        acct_total += trn.amount

    for trn in transaction.envelope_transactions:
        if currency != trn.envelope.currency:
            return "Currency conversion would be required but is not supported"
        env_total += trn.amount

    if acct_total - env_total != create_money(0, currency):
        return "Account split sum less the envelope split sum must equal zero"

    return None


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
    return create_money(query.scalar(), currency)


