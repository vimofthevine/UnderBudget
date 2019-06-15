import React from 'react';
import { Route, Switch } from 'react-router-dom';
import PropTypes from 'prop-types';
import withStyles from '@material-ui/core/styles/withStyles';
import AppBar from './components/AppBar/AppBar';
import Dashboard from './components/Dashboard/Dashboard';
import Drawer from './components/Drawer/AppDrawer';
import Ledgers from './components/Ledgers/Ledgers';
import UserMenu from './components/UserMenu/UserMenu';
import withAuth from './components/withAuth';

const styles = theme => ({
  toolbar: theme.mixins.toolbar,

  main: {
    flexGrow: 1,
    padding: theme.spacing.unit * 3,
  },
});

class App extends React.Component {
  static propTypes = {
    classes: PropTypes.object.isRequired,
    history: PropTypes.object.isRequired,
  };

  state = {
    drawerOpen: false,
    userMenuAnchor: null,
  };

  handleDrawerOpen = () => {
    this.setState({ drawerOpen: true });
  };

  handleDrawerClose = () => {
    this.setState({ drawerOpen: false });
  };

  handleUserMenuOpen = (event) => {
    this.setState({ userMenuAnchor: event.currentTarget });
  };

  handleUserMenuClose = () => {
    this.setState({ userMenuAnchor: null });
  };

  render() {
    const { classes, history } = this.props;
    const { drawerOpen, userMenuAnchor } = this.state;
    return (
      <React.Fragment>
        <AppBar
          drawerOpen={drawerOpen}
          onDrawerOpen={this.handleDrawerOpen}
          onUserMenu={this.handleUserMenuOpen}
        />
        <Drawer
          history={history}
          open={drawerOpen}
          onDrawerClose={this.handleDrawerClose}
        />
        <UserMenu
          history={history}
          anchor={userMenuAnchor}
          onClose={this.handleUserMenuClose}
        />
        <main className={classes.main}>
          <div className={classes.toolbar} />
          <Switch>
            <Route path='/dashboard' component={Dashboard} />
            <Route path='/ledgers' component={Ledgers} />
          </Switch>
        </main>
      </React.Fragment>
    );
  }
}

export default withAuth(withStyles(styles)(App));
