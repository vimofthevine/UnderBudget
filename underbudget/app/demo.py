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

from sqlalchemy import create_engine

from underbudget import db
from underbudget.ledger import model as ledger


def setup(verbose=False):
    db.open('sqlite://', verbose)
    session = db.Session()

    ledger.init(session)
    root_acct = ledger.get_root_account(session)

    # Don't populate in case real data was accidentally given
    if len(root_acct.children) == 0:
        print('Populating DB with demo data')

        # --- Accounts

        acct1 = ledger.Account(name='Credit Cards', parent=root_acct)
        acct1a = ledger.Account(name='Red Card', parent=acct1)
        acct1b = ledger.Account(name='Blue Card', parent=acct1)
        acct2 = ledger.Account(name='Bank', parent=root_acct)
        acct3 = ledger.Account(name='Cash', parent=root_acct)

        session.add(acct1)
        session.add(acct1a)
        session.add(acct1b)
        session.add(acct2)
        session.add(acct3)

        # --- Envelopes

        root_env = ledger.get_root_envelope(session)

        env1 = ledger.Envelope(name='Food', parent=root_env)
        env2 = ledger.Envelope(name='Utilities', parent=root_env)
        env2a = ledger.Envelope(name='Rent', parent=env2)
        env2b = ledger.Envelope(name='Electric', parent=env2)
        env3 = ledger.Envelope(name='Gifts', parent=root_env)
        env4 = ledger.Envelope(name='Unallocated', parent=root_env)

        session.add(env1)
        session.add(env2)
        session.add(env2a)
        session.add(env2b)
        session.add(env3)
        session.add(env4)

        # --- Transactions

        trn1 = ledger.Transaction(date=date(2017, 2, 25), payee='electric bill')
        trn1.account_transactions.append(ledger.AccountTransaction(account=acct1a, amount=-52.33))
        trn1.envelope_transactions.append(ledger.EnvelopeTransaction(envelope=env2b, amount=-52.33))
        print(ledger.validate(trn1))

        session.add(trn1)

        # --- Done

        session.commit()
