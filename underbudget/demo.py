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

from underbudget import db
from underbudget.ledger import model as ledger


def add_if_valid(session, transaction):
    """Validates the transaction and adds it to the session"""
    error = ledger.validate(transaction)
    if error:
        print(error)
    else:
        session.add(transaction)


def setup(verbose=False):
    """Sets up the demo database"""
    db.open('sqlite://', verbose)
    session = db.Session()

    ledger.init(session)
    root_acct = ledger.get_root_account(session)

    # Don't populate in case real data was accidentally given
    if len(root_acct.children) == 0:
        print('Populating DB with demo data')

        # --- Accounts

        credit_cards = ledger.Account(name='Credit Cards', parent=root_acct)
        red_card = ledger.Account(name='Red Card', parent=credit_cards)
        blue_card = ledger.Account(name='Blue Card', parent=credit_cards)
        bank = ledger.Account(name='Bank', parent=root_acct)
        cash = ledger.Account(name='Cash', parent=root_acct)

        session.add(credit_cards)
        session.add(red_card)
        session.add(blue_card)
        session.add(bank)
        session.add(cash)

        # --- Envelopes

        root_env = ledger.get_root_envelope(session)

        food = ledger.Envelope(name='Food', parent=root_env)
        utilities = ledger.Envelope(name='Utilities', parent=root_env)
        rent = ledger.Envelope(name='Rent', parent=utilities)
        electric = ledger.Envelope(name='Electric', parent=utilities)
        gifts = ledger.Envelope(name='Gifts', parent=root_env)
        unallocated = ledger.Envelope(name='Unallocated', parent=root_env)

        session.add(food)
        session.add(utilities)
        session.add(rent)
        session.add(electric)
        session.add(gifts)
        session.add(unallocated)

        # --- Transactions

        trn1 = ledger.Transaction(date=date(2017, 2, 25), payee='electric bill')
        trn1.account_transactions.append(ledger.AccountTransaction(account=red_card, amount=-52.33))
        trn1.envelope_transactions.append(ledger.EnvelopeTransaction(envelope=electric,
                                                                     amount=-52.33))
        add_if_valid(session, trn1)

        trn2 = ledger.Transaction(date=date(2017, 2, 24), payee='Grocer')
        trn2.account_transactions.append(ledger.AccountTransaction(account=cash, amount=-100))
        trn2.envelope_transactions.append(ledger.EnvelopeTransaction(envelope=gifts, amount=-20,
                                                                     memo='Flowers'))
        trn2.envelope_transactions.append(ledger.EnvelopeTransaction(envelope=food, amount=-80))
        add_if_valid(session, trn2)

        trn3 = ledger.Transaction(date=date(2017, 2, 24), payee='Cash withdrawal')
        trn3.account_transactions.append(ledger.AccountTransaction(account=bank, amount=-150))
        trn3.account_transactions.append(ledger.AccountTransaction(account=cash, amount=150))
        add_if_valid(session, trn3)

        trn4 = ledger.Transaction(date=date(2017, 2, 23), payee='payday')
        trn4.account_transactions.append(ledger.AccountTransaction(account=bank, amount=350))
        trn4.envelope_transactions.append(ledger.EnvelopeTransaction(envelope=food, amount=100))
        trn4.envelope_transactions.append(ledger.EnvelopeTransaction(envelope=gifts, amount=25))
        trn4.envelope_transactions.append(ledger.EnvelopeTransaction(envelope=electric, amount=55))
        trn4.envelope_transactions.append(ledger.EnvelopeTransaction(envelope=rent, amount=120))
        trn4.envelope_transactions.append(ledger.EnvelopeTransaction(envelope=unallocated,
                                                                     amount=50))

        # --- Done

        session.commit()
