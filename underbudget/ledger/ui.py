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

from PyQt5.QtCore import pyqtSignal
from PyQt5.QtCore import Qt
from PyQt5.QtCore import QAbstractItemModel
from PyQt5.QtCore import QAbstractTableModel
from PyQt5.QtCore import QEvent
from PyQt5.QtCore import QItemSelectionModel
from PyQt5.QtCore import QModelIndex
from PyQt5.QtCore import QSortFilterProxyModel
from PyQt5.QtWidgets import QAction
from PyQt5.QtWidgets import QDialog
from PyQt5.QtWidgets import QDialogButtonBox
from PyQt5.QtWidgets import QFormLayout
from PyQt5.QtWidgets import QHeaderView
from PyQt5.QtWidgets import QLineEdit
from PyQt5.QtWidgets import QMenu
from PyQt5.QtWidgets import QSplitter
from PyQt5.QtWidgets import QTableView
from PyQt5.QtWidgets import QTreeView

from underbudget import db
from underbudget.ledger import model as ledger


class TreeView(QTreeView):
    """Customized tree view to display accounts and envelopes"""

    select_item = pyqtSignal(['QModelIndex', 'QModelIndex'])
    create_item = pyqtSignal(['QModelIndex'])
    modify_item = pyqtSignal(['QModelIndex'])
    delete_item = pyqtSignal(['QModelIndex'])

    def __init__(self):
        super().__init__()
        self._filter = QSortFilterProxyModel()
        self.setModel(self._filter)
        self._filter.sort(0)

        self.setContextMenuPolicy(Qt.DefaultContextMenu)
        self.setSelectionBehavior(self.SelectRows)
        self.setSelectionMode(self.SingleSelection)

        self.doubleClicked.connect(
            lambda index: self.modify_item.emit(self._filter.mapToSource(index)))

        self.installEventFilter(self)

    def set_model(self, model):
        """Sets the source model for the tree"""
        self._filter.setSourceModel(model)
        model.modelReset.connect(lambda: self.expandAll())
        # Give the first column the most weight
        self.header().setSectionResizeMode(QHeaderView.ResizeToContents)
        self.header().setSectionResizeMode(0, QHeaderView.Stretch)
        # Can't set this up until after the model has been set
        self.selectionModel().currentRowChanged.connect(
            lambda current, previous: self.select_item.emit(self._filter.mapToSource(current),
                                                            self._filter.mapToSource(previous)))
        self.expandAll()

    def select(self, index):
        """Selects the given index"""
        self.selectionModel().setCurrentIndex(self._filter.mapFromSource(index),
                                              QItemSelectionModel.SelectCurrent)

    def contextMenuEvent(self, event):
        event_index = self.indexAt(event.pos())
        menu = QMenu()

        if event_index.isValid():
            index = self._filter.mapToSource(event_index)

            mod = QAction(self.tr('Edit'))
            mod.triggered.connect(lambda: self.modify_item.emit(index))

            add = QAction(self.tr('Add child'))
            add.triggered.connect(lambda: self.create_item.emit(index))

            del_ = QAction(self.tr('Delete'))
            del_.triggered.connect(lambda: self.delete_item.emit(index))

            menu.addAction(mod)
            menu.addAction(add)
            menu.addAction(del_)
        else:
            add = QAction(self.tr('Add top-level item'))
            add.triggered.connect(lambda: self.create_item.emit(QModelIndex()))
            menu.addAction(add)

        menu.exec(event.globalPos())

    def eventFilter(self, obj, event):
        index = self.currentIndex()
        if index.isValid() and event.type() == QEvent.KeyPress:
            key = event.key()
            if key == Qt.Key_Enter or key == Qt.Key_Return:
                self.modify_item.emit(self._filter.mapToSource(index))
            elif key == Qt.Key_Delete:
                self.delete_item.emit(self._filter.mapToSource(index))
            elif key == Qt.Key_A:
                self.create_item.emit(self._filter.mapToSource(index))
        return super().eventFilter(obj, event)


class TransactionView(QTableView):
    """Customized table view for transactions"""

    modify_item = pyqtSignal(['QModelIndex'])
    duplicate_item = pyqtSignal(['QModelIndex'])
    delete_item = pyqtSignal(['QModelIndex'])

    def __init__(self):
        super().__init__()
        self._filter = QSortFilterProxyModel()
        self.setModel(self._filter)
        self.setSortingEnabled(True)

        self.setSelectionBehavior(self.SelectRows)
        self.setSelectionMode(self.SingleSelection)

        self.setAlternatingRowColors(True)

        self.doubleClicked.connect(
            lambda index: self.modify_item.emit(self._filter.mapToSource(index)))

        self.installEventFilter(self)

    def set_model(self, model):
        """Sets the source model for the table"""
        self._filter.setSourceModel(model)
        self.sortByColumn(model.DATE, Qt.AscendingOrder)
        # Give the payee and memo columns the most weight
        self.horizontalHeader().setSectionResizeMode(QHeaderView.ResizeToContents)
        self.horizontalHeader().setSectionResizeMode(model.PAYEE, QHeaderView.Stretch)
        self.horizontalHeader().setSectionResizeMode(model.MEMO, QHeaderView.Stretch)

    def contextMenuEvent(self, event):
        event_index = self.indexAt(event.pos())
        if event_index.isValid():
            index = self._filter.mapToSource(event_index)

            mod = QAction(self.tr('Edit'))
            mod.triggered.connect(lambda: self.modify_item.emit(index))

            dup = QAction(self.tr('Duplicate'))
            dup.triggered.connect(lambda: self.duplicate_item.emit(index))

            del_ = QAction(self.tr('Delete'))
            del_.triggered.connect(lambda: self.delete_item.emit(index))

            menu = QMenu()
            menu.addAction(mod)
            menu.addAction(dup)
            menu.addAction(del_)
            menu.exec(event.globalPos())

    def eventFilter(self, obj, event):
        index = self.currentIndex()
        if index.isValid() and event.type() == QEvent.KeyPress:
            key = event.key()
            if key == Qt.Key_Enter or key == Qt.Key_Return:
                self.modify_item.emit(self._filter.mapToSource(index))
            elif key == Qt.Key_Delete:
                self.delete_item.emit(self._filter.mapToSource(index))
            elif key == Qt.Key_D:
                self.duplicate_item.emit(self._filter.mapToSource(index))
        return super().eventFilter(obj, event)


class EntityModel(QAbstractItemModel):
    """Entity item model"""

    NAME = 0
    BALANCE = 1

    def __init__(self):
        super().__init__()
        self._headers = [self.tr('Name'), self.tr('Balance')]
        self._root = None
        self._cache = {}

    def set_root(self, root):
        """Updates the entity tree wrapped by the model"""
        self.beginResetModel()
        self._cache.clear()
        self._root = root
        self._add_to_cache(root)
        self.endResetModel()

    def get(self, index):
        """Retrieves the entity identified by the index"""
        if not self._root:
            return None
        if not index.isValid():
            return self._root
        return self._cache[index.internalId()]

    def add(self, entity, index=None):
        """Adds a new entity under the given parent index"""
        if self._root:
            if not index:
                index = QModelIndex()
            rows = self.rowCount(index)
            self.beginInsertRows(index, rows, rows)
            with db.open_ui_session() as session:
                parent = self.get(index)
                parent.children.append(entity)
                session.add(entity)
            if entity.id:
                self._add_to_cache(entity)
            self.endInsertRows()

    def update(self, index):
        """Updates the specified index"""
        self.dataChanged(self.index(index.row(), 0, index.parent()),
                         self.index(index.row(), self.columnCount(index) - 1, index.parent()))

    def get_balance(self, entity):
        """Retrieves the balance of the given entity"""
        return None

    def columnCount(self, parent=None):
        return len(self._headers)

    def headerData(self, section, orientation, role):
        if orientation == Qt.Horizontal and role == Qt.DisplayRole:
            return self._headers[section]
        return None

    def data(self, index, role):
        if not self._root or not index.isValid():
            return None

        if role != Qt.DisplayRole:
            return None

        entity = self.get(index)
        if not entity:
            return None

        col = index.column()
        if col == self.NAME:
            return entity.name
        elif col == self.BALANCE:
            if len(entity.children) > 0:
                return None
            else:
                return self.get_balance(entity)
        else:
            return None

    def rowCount(self, parent):
        if not self._root or parent.column() > 0:
            return 0
        entity = self.get(parent)
        if entity:
            return len(entity.children)
        else:
            return 0

    def index(self, row, column, parent):
        if not self._root or not self.hasIndex(row, column, parent):
            return QModelIndex()

        entity = self.get(parent)
        if not entity:
            return QModelIndex()
        else:
            children = entity.children

        if row < 0 or row >= len(children):
            return QModelIndex()

        return self.createIndex(row, column, children[row].id)

    def parent(self, child):
        if not self._root or not child.isValid():
            return QModelIndex()

        entity = self.get(child)
        if not entity:
            return QModelIndex()

        if not entity.parent:
            return QModelIndex()
        parent = entity.parent

        if not parent.parent:
            return QModelIndex()
        grandparent = parent.parent

        return self.createIndex(grandparent.children.index(parent), 0, parent.id)

    def _add_to_cache(self, entity):
        self._cache[entity.id] = entity
        for child in entity.children:
            self._add_to_cache(child)


class AccountModel(EntityModel):

    def get_balance(self, entity):
        return ledger.get_balance(db.ui_session, date=date.today(), account=entity).format('en_US')


class EnvelopeModel(EntityModel):

    def get_balance(self, entity):
        return ledger.get_balance(db.ui_session, date=date.today(), envelope=entity).format('en_US')


class TransactionModel(QAbstractTableModel):
    """Transaction model"""

    DATE = 0
    PAYEE = 1
    MEMO = 2
    DEBIT = 3
    CREDIT = 4
    BALANCE = 5

    def __init__(self):
        super().__init__()
        self._headers = [self.tr('Date'), self.tr('Payee'), self.tr('Memo'), self.tr('Debit'),
                         self.tr('Credit'), self.tr('Balance')]
        self._transactions = []

    def get_transactions(self, entity):
        """Retrieves transactions for the given entity"""
        return []

    def filter(self, entity):
        self.beginResetModel()
        if entity:
            self._transactions = self.get_transactions(entity)
        else:
            self._transactions = []
        self.endResetModel()

    def columnCount(self, parent=None):
        return len(self._headers)

    def headerData(self, section, orientation, role):
        if orientation == Qt.Horizontal and role == Qt.DisplayRole:
            return self._headers[section]
        return None

    def data(self, index, role):
        if not index.isValid():
            return None

        if role != Qt.DisplayRole and role != Qt.EditRole:
            return None

        row = index.row()
        if row < 0 or row >= len(self._transactions):
            return None

        trn = self._transactions[row]
        col = index.column()
        if col == self.DATE:
            return str(trn.transaction.date)
        elif col == self.PAYEE:
            return trn.transaction.payee
        elif col == self.MEMO:
            return trn.memo
        elif col == self.DEBIT and trn.amount.amount >= 0:
            return trn.amount.format('en_US')
        elif col == self.CREDIT and trn.amount.amount < 0:
            return (-trn.amount).format('en_US')
        elif col == self.BALANCE:
            return trn.balance.format('en_US')
        return None

    def rowCount(self, parent):
        if parent.column() > 0:
            return 0
        return len(self._transactions)


class AccountTransactionModel(TransactionModel):

    def get_transactions(self, entity):
        return ledger.get_account_transactions(db.Session(), entity)


class EnvelopeTransactionModel(TransactionModel):

    def get_transactions(self, entity):
        return ledger.get_envelope_transactions(db.Session(), entity)


class EntityDetailsDialog(QDialog):
    """Entity details dialog"""

    def __init__(self, type, model, parent):
        super().__init__(parent)
        self._model = model
        self._parent_index = None
        self._entity_index = None
        self._entity = None

        self._name = QLineEdit()
        self._name.setPlaceholderText(self.tr('{0} name'.format(type)))

        self._buttons = QDialogButtonBox(QDialogButtonBox.Save | QDialogButtonBox.Reset |
                                         QDialogButtonBox.Cancel)
        self._buttons.clicked.connect(self.clicked)

        form = QFormLayout()
        form.addRow(self.tr('Name'), self._name)
        form.addRow('', self._buttons)
        self.setLayout(form)
        self.setWindowTitle(self.tr(type))

    def reset(self, entity, index, parent=None):
        self._entity = entity
        self._entity_index = index
        self._parent_index = parent
        self._name.setText(self._entity.name)

    def clicked(self, button):
        if button is self._buttons.button(QDialogButtonBox.Save):
            self._entity.name = self._name.text()
            if self._parent_index:
                self._model.add(self._entity, self._parent_index)
            self.accept()
        elif button is self._buttons.button(QDialogButtonBox.Reset):
            self._name.setText(self._entity.name if self._entity else '')
        elif button is self._buttons.button(QDialogButtonBox.Cancel):
            self.reject()


class EntityView(QSplitter):
    """Entity (envelope or account) widget with hierarchy tree and transaction table"""

    def __init__(self, entities, transactions):
        super().__init__()

        self._type = 'Account' if type(entities) is AccountModel else 'Envelope'

        self._entities = entities
        self._transactions = transactions

        self._tree = TreeView()
        self._tree.set_model(self._entities)

        self._table = TransactionView()
        self._table.set_model(self._transactions)

        self._details = EntityDetailsDialog(self._type, self._entities, self)

        self.addWidget(self._tree)
        self.addWidget(self._table)
        # Give transaction list stretch priority
        self.setStretchFactor(1, 1)

        self._tree.select_item.connect(self.set_transaction_filter)
        self._tree.create_item.connect(self.create)
        #self._tree.modify_item = pyqtSignal(['QModelIndex'])
        #self._tree.delete_item = pyqtSignal(['QModelIndex'])

    def set_transaction_filter(self, current, previous):
        entity = self._entities.get(current)
        if entity:
            self._transactions.filter(entity)
            self._table.scrollToBottom()

    def create(self, parent):
        entity = ledger.Account() if type(self._entities) is AccountModel else ledger.Envelope()
        self._details.reset(entity, None, parent)
        self._details.show()


