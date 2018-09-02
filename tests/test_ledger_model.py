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


import unittest

from underbudget import db
from underbudget.ledger import model as ledger


class Base(unittest.TestCase):
    """Base test case"""

    def setUp(self):
        db.open('sqlite://', False)
        self.session = db.Session()
        ledger.init(self.session)

    def populate(self):
        self.uah = ledger.Currency(id=2, code='UAH')
        self.bank = ledger.Account(name='bank')
        self.credit = ledger.Account(name='credit card')
        self.food = ledger.Envelope(name='food')
        self.utilities = ledger.Envelope(name='utilities')
        self.session.add(self.uah)
        self.session.add(self.bank)
        self.session.add(self.credit)
        self.session.add(self.food)
        self.session.add(self.utilities)
        self.session.commit()


class TransactionValidation(Base):
    """Unit test case for transaction validation"""

    def test_invalid_when_no_splits(self):
        trn = ledger.Transaction()
        self.assertIsNotNone(ledger.validate(trn))

    def test_invalid_when_multiple_account_and_envelope_splits(self):
        trn = ledger.Transaction()
        trn.account_transactions.append(ledger.AccountTransaction())
        trn.account_transactions.append(ledger.AccountTransaction())
        trn.envelope_transactions.append(ledger.EnvelopeTransaction())
        trn.envelope_transactions.append(ledger.EnvelopeTransaction())
        self.assertIsNotNone(ledger.validate(trn))

    def test_invalid_when_multiple_account_currencies(self):
        self.populate()
        foreign = ledger.Account(name='foreign', currency=self.uah)
        trn = ledger.Transaction()
        trn.account_transactions.append(ledger.AccountTransaction(account=self.bank))
        trn.account_transactions.append(ledger.AccountTransaction(account=foreign))
        self.assertIsNotNone(ledger.validate(trn))

    def test_invalid_when_multiple_envelope_currencies(self):
        self.populate()
        foreign = ledger.Envelope(name='foreign', currency=self.uah)
        trn = ledger.Transaction()
        trn.envelope_transactions.append(ledger.EnvelopeTransaction(envelope=self.food))
        trn.envelope_transactions.append(ledger.EnvelopeTransaction(envelope=foreign))
        self.assertIsNotNone(ledger.validate(trn))

    def test_invalid_when_sum_not_zero(self):
        self.populate()
        trn = ledger.Transaction()
        trn.account_transactions.append(ledger.AccountTransaction(account=self.bank, amount=100))
        trn.envelope_transactions.append(ledger.EnvelopeTransaction(envelope=self.utilities, amount=99))
        self.assertIsNotNone(ledger.validate(trn))

    def test_valid(self):
        self.populate()
        trn = ledger.Transaction()
        trn.account_transactions.append(ledger.AccountTransaction(account=self.bank, amount=100))
        trn.envelope_transactions.append(ledger.EnvelopeTransaction(envelope=self.utilities, amount=100))
        self.assertIsNone(ledger.validate(trn))


class AccountCascadeOperations(Base):
    """Unit test case for cascading account operations"""

    def test_delete_currency_sets_accounts_to_default(self):
        self.populate()
        foreign = ledger.Account(name='foreign', currency=self.uah)
        self.session.add(foreign)
        self.session.commit()
        self.assertEqual(foreign.currency.code, 'UAH')
        self.session.delete(self.uah)
        self.session.commit()
        self.assertEqual(foreign.currency.code, 'USD')

    def test_delete_account_deletes_children(self):
        root = ledger.get_root_account(self.session)
        p1 = ledger.Account(name='parent 1', parent=root)
        c1 = ledger.Account(name='child 1', parent=p1)
        gc1 = ledger.Account(name='grandchild 1', parent=c1)
        gc2 = ledger.Account(name='grandchild 2', parent=c1)
        c2 = ledger.Account(name='child 2', parent=p1)
        gc3 = ledger.Account(name='grandchild 3', parent=c2)
        self.session.add(p1)
        self.session.commit()
        c1_id = c1.id
        gc1_id = gc1.id
        gc2_id = gc2.id
        self.session.delete(c1)
        self.assertIsNone(ledger.get_account(self.session, c1_id))
        self.assertIsNone(ledger.get_account(self.session, gc1_id))
        self.assertIsNone(ledger.get_account(self.session, gc2_id))
        self.assertIsNotNone(ledger.get_account(self.session, p1.id))
        self.assertIsNotNone(ledger.get_account(self.session, c2.id))
        self.assertIsNotNone(ledger.get_account(self.session, gc3.id))


class EnvelopeCascadeOperations(Base):
    """Unit test case for cascading enveloe operations"""

    def test_delete_currency_sets_envelopes_to_default(self):
        self.populate()
        foreign = ledger.Envelope(name='foreign', currency=self.uah)
        self.session.add(foreign)
        self.session.commit()
        self.assertEqual(foreign.currency.code, 'UAH')
        self.session.delete(self.uah)
        self.session.commit()
        self.assertEqual(foreign.currency.code, 'USD')

    def test_delete_envelope_deletes_children(self):
        root = ledger.get_root_envelope(self.session)
        p1 = ledger.Envelope(name='parent 1', parent=root)
        c1 = ledger.Envelope(name='child 1', parent=p1)
        gc1 = ledger.Envelope(name='grandchild 1', parent=c1)
        gc2 = ledger.Envelope(name='grandchild 2', parent=c1)
        c2 = ledger.Envelope(name='child 2', parent=p1)
        gc3 = ledger.Envelope(name='grandchild 3', parent=c2)
        self.session.add(p1)
        self.session.commit()
        c1_id = c1.id
        gc1_id = gc1.id
        gc2_id = gc2.id
        self.session.delete(c1)
        self.assertIsNone(ledger.get_envelope(self.session, c1_id))
        self.assertIsNone(ledger.get_envelope(self.session, gc1_id))
        self.assertIsNone(ledger.get_envelope(self.session, gc2_id))
        self.assertIsNotNone(ledger.get_envelope(self.session, p1.id))
        self.assertIsNotNone(ledger.get_envelope(self.session, c2.id))
        self.assertIsNotNone(ledger.get_envelope(self.session, gc3.id))


if __name__ == '__main__':
    unittest.main()
