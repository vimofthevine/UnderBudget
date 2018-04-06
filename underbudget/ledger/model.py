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

from sqlalchemy import Boolean
from sqlalchemy import Column
from sqlalchemy import ForeignKey
from sqlalchemy import Integer
from sqlalchemy import String
from sqlalchemy.ext.declarative import declarative_base
from sqlalchemy.orm import aliased
from sqlalchemy.orm import backref
from sqlalchemy.orm import relationship
from sqlalchemy.orm.exc import NoResultFound


Base = declarative_base()


class Currency(Base):
    """Model of a single monetary currency."""

    __tablename__ = 'currency'

    id = Column(Integer, primary_key=True)
    code = Column(String)
    external_id = Column('ext_id', String)


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

    def __composite_values__(self):
        return self.amount, self._currency_obj


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
    parent_id = Column(Integer, ForeignKey('account.id'))
    children = relationship('Account', cascade='all, delete, delete-orphan',
                            backref=backref('parent', remote_side=[id]))

    def __repr__(self):
        return '{0} ({1})'.format(self.name, self.id)


class AccountRepository(object):
    """Repository of accounts"""

    last_error = ''

    def __init__(self, session):
        self.session = session
        Base.metadata.create_all(session.bind)
        if not session.query(Account).filter(Account.id == 1).one_or_none():
            root = Account(id=1, name='root')
            session.add(root)
            session.commit()

    def commit(self):
        self.session.commit()

    def create(self, account, parent):
        account.parent_id = parent.id
        self.session.add(account)

    def get_account(self, id):
        try:
            return self.session.query(Account).filter(Account.id == id).one()
        except NoResultFound:
            self.last_error = 'No account exists with ID {0}'.format(id)
            return Account()

    def get_leaf_accounts(self):
        # Perform a LEFT JOIN where the right side is the accounts with a parent
        # matching the left side, then return only the results where there is no
        # valid right side (i.e., no children)
        right = aliased(Account)
        return self.session.query(Account)\
            .join(right, right.parent_id == Account.id, isouter=True)\
            .filter(right.id.is_(None)).all()

    def get_root(self):
        return self.get_account(1)

    def remove(self, account):
        self.session.delete(account)


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
    parent_id = Column(Integer, ForeignKey('envelope.id'))
    children = relationship('Envelope', cascade='all, delete, delete-orphan',
                            backref=backref('parent', remote_side=[id]))

    def __repr__(self):
        return '{0} ({1})'.format(self.name, self.id)


class EnvelopeRepository(object):
    """Repository of envelopes"""

    last_error = ''

    def __init__(self, session):
        self.session = session
        Base.metadata.create_all(session.bind)
        if not session.query(Envelope).filter(Envelope.id == 1).one_or_none():
            root = Envelope(id=1, name='root')
            session.add(root)
            session.commit()

    def commit(self):
        self.session.commit()

    def create(self, envelope, parent):
        envelope.parent_id = parent.id
        self.session.add(envelope)

    def get_envelope(self, id):
        try:
            return self.session.query(Envelope).filter(Envelope.id == id).one()
        except NoResultFound:
            self.last_error = 'No envelope exists with ID {0}'.format(id)
            return Envelope()

    def get_leaf_envelopes(self):
        # Perform a LEFT JOIN where the right side is the envelopes with a parent
        # matching the left side, then return only the results where there is no
        # valid right side (i.e., no children)
        right = aliased(Envelope)
        return self.session.query(Envelope) \
            .join(right, right.parent_id == Envelope.id, isouter=True) \
            .filter(right.id.is_(None)).all()

    def get_root(self):
        return self.get_envelope(1)

    def remove(self, envelope):
        self.session.delete(envelope)


