import React from 'react'
import { Route, Switch } from 'react-router-dom';
import withStyles from '@material-ui/core/styles/withStyles'
import AppBar from './components/AppBar/AppBar'
import Dashboard from './components/Dashboard/Dashboard'
import Drawer from './components/Drawer/AppDrawer'
import Ledgers from './components/Ledgers/Ledgers'
import UserMenu from './components/UserMenu/UserMenu'
import withAuth from './components/withAuth'

const styles = theme => ({
  toolbar: theme.mixins.toolbar,
    
  main: {
    flexGrow: 1,
    padding: theme.spacing.unit * 3
  }
})

class App extends React.Component {
  state = {
    drawerOpen: false,
    userMenuAnchor: null
  }

  handleDrawerOpen = () => {
    this.setState({ drawerOpen: true })
  }
  
  handleDrawerClose = () => {
    this.setState({ drawerOpen: false })
  }
  
  handleUserMenuOpen = event => {
    this.setState({ userMenuAnchor: event.currentTarget })
  }
  
  handleUserMenuClose = () => {
    this.setState({ userMenuAnchor: null })
  }

  render() {
    return (
      <React.Fragment>
        <AppBar
          drawerOpen={this.state.drawerOpen}
          onDrawerOpen={this.handleDrawerOpen}
          onUserMenu={this.handleUserMenuOpen}
        />
        <Drawer
          history={this.props.history}
          open={this.state.drawerOpen}
          onDrawerClose={this.handleDrawerClose}
        />
        <UserMenu
          history={this.props.history}
          anchor={this.state.userMenuAnchor}
          onClose={this.handleUserMenuClose}
        />
        <main className={this.props.classes.main}>
          <div className={this.props.classes.toolbar} />
          <Switch>
           <Route path="/dashboard" component={Dashboard} />
           <Route path="/ledgers" component={Ledgers} />
          </Switch>
        </main>
      </React.Fragment>
    );
  }
}

export default withAuth(withStyles(styles)(App))