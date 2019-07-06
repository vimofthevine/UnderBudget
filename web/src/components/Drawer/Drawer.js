import React from 'react';
import PropTypes from 'prop-types';
import { connect } from 'react-redux';
import { push } from 'connected-react-router';
import AccountIcon from '@material-ui/icons/AccountBalance';
import DashboardIcon from '@material-ui/icons/Dashboard';
import Divider from '@material-ui/core/Divider';
import EnvelopeIcon from '@material-ui/icons/Mail';
import ExpenseIcon from '@material-ui/icons/ShoppingCart';
import IncomeIcon from '@material-ui/icons/MonetizationOn';
import LedgerIcon from '@material-ui/icons/Folder';
import List from '@material-ui/core/List';
import ListItem from '@material-ui/core/ListItem';
import ListItemIcon from '@material-ui/core/ListItemIcon';
import ListItemText from '@material-ui/core/ListItemText';
import ReportIcon from '@material-ui/icons/InsertChart';
import BaseDrawer from './BaseDrawer';

const DrawerItem = ({
  dest,
  icon,
  onClick,
  text,
}) => (
  <ListItem button key={text} onClick={() => onClick(dest)}>
    <ListItemIcon>{icon}</ListItemIcon>
    <ListItemText primary={text} />
  </ListItem>
);

DrawerItem.propTypes = {
  dest: PropTypes.string.isRequired,
  icon: PropTypes.element.isRequired,
  onClick: PropTypes.func.isRequired,
  text: PropTypes.string.isRequired,
};

const Drawer = ({ navigate }) => (
  <BaseDrawer>
    <List>
      <DrawerItem
        dest='/dashboard'
        icon={<DashboardIcon />}
        onClick={navigate}
        text='Dashboard'
      />
      <DrawerItem
        dest='/ledgers'
        icon={<LedgerIcon />}
        onClick={navigate}
        text='Ledgers'
      />
    </List>
    <Divider />
    <List>
      <DrawerItem
        dest='/accounts'
        icon={<AccountIcon />}
        onClick={navigate}
        text='Accounts'
      />
      <DrawerItem
        dest='/envelopes'
        icon={<EnvelopeIcon />}
        onClick={navigate}
        text='Envelopes'
      />
      <DrawerItem
        dest='/incomes'
        icon={<IncomeIcon />}
        onClick={navigate}
        text='Incomes'
      />
      <DrawerItem
        dest='/expenses'
        icon={<ExpenseIcon />}
        onClick={navigate}
        text='Expenses'
      />
    </List>
    <Divider />
    <List>
      <DrawerItem
        dest='/reports'
        icon={<ReportIcon />}
        onClick={navigate}
        text='Reports'
      />
    </List>
  </BaseDrawer>
);

Drawer.propTypes = {
  navigate: PropTypes.func.isRequired,
};

export default connect(
  null,
  { navigate: push },
)(Drawer);
