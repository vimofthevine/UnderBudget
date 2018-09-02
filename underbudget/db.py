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

from contextlib import contextmanager

from sqlalchemy import create_engine
from sqlalchemy.orm import sessionmaker
from sqlalchemy.ext.declarative import declarative_base


Base = declarative_base()

Session = sessionmaker()


def open(loc, verbose=False):
    """Opens the database from the given location and configures the session factory"""
    engine = create_engine(loc, echo=verbose)
    Session.configure(bind=engine)
    if 'sqlite' in loc:
        session = Session()
        session.execute('PRAGMA foreign_keys=ON;')
        session.commit()
    Base.metadata.create_all(engine)


@contextmanager
def session_scope():
    """Provide transactional scope"""
    session = Session()
    try:
        yield session
        session.commit()
    except:
        session.rollback()
        raise
    finally:
        session.close()
