import React from 'react'
import AccountIcon from '@material-ui/icons/AccountBalance'
import DashboardIcon from '@material-ui/icons/Dashboard'
import Divider from '@material-ui/core/Divider'
import EnvelopeIcon from '@material-ui/icons/Mail'
import ExpenseIcon from '@material-ui/icons/ShoppingCart'
import IncomeIcon from '@material-ui/icons/MonetizationOn'
import LedgerIcon from '@material-ui/icons/Folder'
import List from '@material-ui/core/List'
import ListItem from '@material-ui/core/ListItem'
import ListItemIcon from '@material-ui/core/ListItemIcon'
import ListItemText from '@material-ui/core/ListItemText'
import ReportIcon from '@material-ui/icons/InsertChart'
import Drawer from './Drawer'

class AppDrawer extends React.Component {
  render() {
    return(
      <Drawer {...this.props}>
        <List>
          <ListItem button key="Dashboard">
            <ListItemIcon><DashboardIcon /></ListItemIcon>
            <ListItemText primary="Dashboard" />
          </ListItem>
          <ListItem button key="Ledgers">
            <ListItemIcon><LedgerIcon /></ListItemIcon>
            <ListItemText primary="Ledgers" />
          </ListItem>
        </List>
        <Divider />
        <List>
          <ListItem button key="Accounts">
            <ListItemIcon><AccountIcon /></ListItemIcon>
            <ListItemText primary="Accounts" />
          </ListItem>
          <ListItem button key="Envelopes">
            <ListItemIcon><EnvelopeIcon /></ListItemIcon>
            <ListItemText primary="Envelopes" />
          </ListItem>
          <ListItem button key="Incomes">
            <ListItemIcon><IncomeIcon /></ListItemIcon>
            <ListItemText primary="Incomes" />
          </ListItem>
          <ListItem button key="Expenses">
            <ListItemIcon><ExpenseIcon /></ListItemIcon>
            <ListItemText primary="Expenses" />
          </ListItem>
        </List>
        <Divider />
        <List>
          <ListItem button key="Reports">
            <ListItemIcon><ReportIcon /></ListItemIcon>
            <ListItemText primary="Reports" />
          </ListItem>
        </List>
      </Drawer>
    )
  }
}

export default AppDrawer