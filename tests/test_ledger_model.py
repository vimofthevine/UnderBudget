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


from datetime import date
import unittest

from money import Money

from underbudget import db
from underbudget.ledger import model as ledger


class Base(unittest.TestCase):
    """Base test case"""

    def setUp(self):
        db.open('sqlite://', False)
        self.session = db.Session()
        ledger.init(self.session)

    def populate(self):
        self.usd = ledger.Currency(id=1, code='USD')
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


class Balance(Base):
    """Unit test case for balance calculations"""

    def populate(self):
        super().populate()
        trn1 = ledger.Transaction(date=date(2018, 8, 31), payee='paycheck')
        trn1.account_transactions.append(
            ledger.AccountTransaction(account=self.bank, amount=100, cleared=True))
        trn1.envelope_transactions.append(ledger.EnvelopeTransaction(envelope=self.food, amount=60))
        trn1.envelope_transactions.append(
            ledger.EnvelopeTransaction(envelope=self.utilities, amount=40))
        self.session.add(trn1)

        trn2 = ledger.Transaction(date=date(2018, 9, 1), payee='grocer')
        trn2.account_transactions.append(
            ledger.AccountTransaction(account=self.credit, amount=-12.75))
        trn2.envelope_transactions.append(
            ledger.EnvelopeTransaction(envelope=self.food, amount=-12.75))
        self.session.add(trn2)

        trn3 = ledger.Transaction(date=date(2018, 9, 2), payee='electric')
        trn3.account_transactions.append(
            ledger.AccountTransaction(account=self.bank, amount=-38.97))
        trn3.envelope_transactions.append(
            ledger.EnvelopeTransaction(envelope=self.utilities, amount=-38.97))
        self.session.add(trn3)

        trn4 = ledger.Transaction(date=date(2018, 9, 3), payee='credit card payment')
        trn4.account_transactions.append(
            ledger.AccountTransaction(account=self.bank, amount=-12))
        trn4.account_transactions.append(
            ledger.AccountTransaction(account=self.credit, amount=12))
        self.session.add(trn4)

        self.session.commit()

    def test_overall_uncleared_balance(self):
        self.populate()
        self.assertEqual(Money(0, 'USD'), ledger.get_balance(self.session, date(2018, 8, 29)))
        self.assertEqual(Money(0, 'USD'), ledger.get_balance(self.session, date(2018, 8, 30)))
        self.assertEqual(Money(100, 'USD'), ledger.get_balance(self.session, date(2018, 8, 31)))
        self.assertEqual(Money(87.25, 'USD'), ledger.get_balance(self.session, date(2018, 9, 1)))
        self.assertEqual(Money(48.28, 'USD'), ledger.get_balance(self.session, date(2018, 9, 2)))
        self.assertEqual(Money(48.28, 'USD'), ledger.get_balance(self.session, date(2018, 9, 3)))
        self.assertEqual(Money(48.28, 'USD'), ledger.get_balance(self.session, date(2018, 9, 4)))

    def test_overall_cleared_balance(self):
        self.populate()
        self.assertEqual(Money(0, 'USD'),
                         ledger.get_balance(self.session, date(2018, 8, 29), cleared=True))
        self.assertEqual(Money(0, 'USD'),
                         ledger.get_balance(self.session, date(2018, 8, 30), cleared=True))
        self.assertEqual(Money(100, 'USD'),
                         ledger.get_balance(self.session, date(2018, 8, 31), cleared=True))
        self.assertEqual(Money(100, 'USD'),
                         ledger.get_balance(self.session, date(2018, 9, 1), cleared=True))
        self.assertEqual(Money(100, 'USD'),
                         ledger.get_balance(self.session, date(2018, 9, 2), cleared=True))
        self.assertEqual(Money(100, 'USD'),
                         ledger.get_balance(self.session, date(2018, 9, 3), cleared=True))
        self.assertEqual(Money(100, 'USD'),
                         ledger.get_balance(self.session, date(2018, 9, 4), cleared=True))

    def test_account_balance(self):
        self.populate()
        self.assertEqual(Money(0, 'USD'),
                         ledger.get_balance(self.session, date(2018, 8, 30), account=self.bank))
        self.assertEqual(Money(100, 'USD'),
                         ledger.get_balance(self.session, date(2018, 8, 31), account=self.bank))
        self.assertEqual(Money(100, 'USD'),
                         ledger.get_balance(self.session, date(2018, 9, 1), account=self.bank))
        self.assertEqual(Money(61.03, 'USD'),
                         ledger.get_balance(self.session, date(2018, 9, 2), account=self.bank))
        self.assertEqual(Money(49.03, 'USD'),
                         ledger.get_balance(self.session, date(2018, 9, 3), account=self.bank))
        self.assertEqual(Money(49.03, 'USD'),
                         ledger.get_balance(self.session, date(2018, 9, 4), account=self.bank))
        self.assertEqual(Money(100, 'USD'),
                         ledger.get_balance(self.session, date(2018, 9, 4), account=self.bank, cleared=True))
        self.assertEqual(Money(0, 'USD'),
                         ledger.get_balance(self.session, date(2018, 8, 30), account=self.credit))
        self.assertEqual(Money(0, 'USD'),
                         ledger.get_balance(self.session, date(2018, 8, 31), account=self.credit))
        self.assertEqual(Money(-12.75, 'USD'),
                         ledger.get_balance(self.session, date(2018, 9, 1), account=self.credit))
        self.assertEqual(Money(-12.75, 'USD'),
                         ledger.get_balance(self.session, date(2018, 9, 2), account=self.credit))
        self.assertEqual(Money(-0.75, 'USD'),
                         ledger.get_balance(self.session, date(2018, 9, 3), account=self.credit))
        self.assertEqual(Money(-0.75, 'USD'),
                         ledger.get_balance(self.session, date(2018, 9, 4), account=self.credit))
        self.assertEqual(Money(0, 'USD'),
                         ledger.get_balance(self.session, date(2018, 9, 4), account=self.credit, cleared=True))

    def test_envelope_balance(self):
        self.populate()
        self.assertEqual(Money(0, 'USD'),
                         ledger.get_balance(self.session, date(2018, 8, 30), envelope=self.food))
        self.assertEqual(Money(60, 'USD'),
                         ledger.get_balance(self.session, date(2018, 8, 31), envelope=self.food))
        self.assertEqual(Money(47.25, 'USD'),
                         ledger.get_balance(self.session, date(2018, 9, 1), envelope=self.food))
        self.assertEqual(Money(47.25, 'USD'),
                         ledger.get_balance(self.session, date(2018, 9, 2), envelope=self.food))
        self.assertEqual(Money(47.25, 'USD'),
                         ledger.get_balance(self.session, date(2018, 9, 3), envelope=self.food))
        self.assertEqual(Money(0, 'USD'),
                         ledger.get_balance(self.session, date(2018, 8, 30), envelope=self.utilities))
        self.assertEqual(Money(40, 'USD'),
                         ledger.get_balance(self.session, date(2018, 8, 31), envelope=self.utilities))
        self.assertEqual(Money(40, 'USD'),
                         ledger.get_balance(self.session, date(2018, 9, 1), envelope=self.utilities))
        self.assertEqual(Money(1.03, 'USD'),
                         ledger.get_balance(self.session, date(2018, 9, 2), envelope=self.utilities))
        self.assertEqual(Money(1.03, 'USD'),
                         ledger.get_balance(self.session, date(2018, 9, 3), envelope=self.utilities))


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
