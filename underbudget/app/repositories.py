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


from sqlalchemy import *
from sqlalchemy.orm import *

from underbudget.ledger import model as ledgermodel


class DatabaseRepositories:
    """Collection of application model SQL repositories."""

    def __init__(self, loc=None, verbose=False):
        self.location = loc
        if not loc:
            path = 'sqlite://'
            self.location = 'in-memory'
        else:
            path = 'sqlite://{0}'.format(loc)
        self.engine = create_engine(path, echo=verbose)
        session_type = sessionmaker(bind=self.engine)
        self.session = session_type()
        print('Opened database file: {0}'.format(self.location))
        self.accounts = ledgermodel.AccountRepository(self.session)
        self.envelopes = ledgermodel.EnvelopeRepository(self.session)
        self.transactions = ledgermodel.TransactionRepository(self.session)
