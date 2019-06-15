import React from 'react';
import PropTypes from 'prop-types';
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
import Drawer from './Drawer';

class AppDrawer extends React.Component {
  handleNav = url => () => {
    const { history } = this.props;
    history.replace(url);
  };

  render() {
    return (
      <Drawer {...this.props}>
        <List>
          <ListItem
            button
            key='Dashboard'
            onClick={this.handleNav('/dashboard')}
          >
            <ListItemIcon>
              <DashboardIcon />
            </ListItemIcon>
            <ListItemText primary='Dashboard' />
          </ListItem>
          <ListItem button key='Ledgers' onClick={this.handleNav('/ledgers')}>
            <ListItemIcon>
              <LedgerIcon />
            </ListItemIcon>
            <ListItemText primary='Ledgers' />
          </ListItem>
        </List>
        <Divider />
        <List>
          <ListItem button key='Accounts' onClick={this.handleNav('/accounts')}>
            <ListItemIcon>
              <AccountIcon />
            </ListItemIcon>
            <ListItemText primary='Accounts' />
          </ListItem>
          <ListItem
            button
            key='Envelopes'
            onClick={this.handleNav('/envelopes')}
          >
            <ListItemIcon>
              <EnvelopeIcon />
            </ListItemIcon>
            <ListItemText primary='Envelopes' />
          </ListItem>
          <ListItem button key='Incomes' onClick={this.handleNav('/incomes')}>
            <ListItemIcon>
              <IncomeIcon />
            </ListItemIcon>
            <ListItemText primary='Incomes' />
          </ListItem>
          <ListItem button key='Expenses' onClick={this.handleNav('/expenses')}>
            <ListItemIcon>
              <ExpenseIcon />
            </ListItemIcon>
            <ListItemText primary='Expenses' />
          </ListItem>
        </List>
        <Divider />
        <List>
          <ListItem button key='Reports' onClick={this.handleNav('/reports')}>
            <ListItemIcon>
              <ReportIcon />
            </ListItemIcon>
            <ListItemText primary='Reports' />
          </ListItem>
        </List>
      </Drawer>
    );
  }
}

AppDrawer.propTypes = {
  history: PropTypes.object.isRequired,
};

export default AppDrawer;
