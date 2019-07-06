import React from 'react';
import PropTypes from 'prop-types';
import classNames from 'classnames';
import { connect } from 'react-redux';
// import AccountCircleIcon from '@material-ui/icons/AccountCircle';
import AppBar from '@material-ui/core/AppBar';
import IconButton from '@material-ui/core/IconButton';
import MenuIcon from '@material-ui/icons/Menu';
import Toolbar from '@material-ui/core/Toolbar';
import Typography from '@material-ui/core/Typography';
import { makeStyles } from '@material-ui/styles';
import { openDrawer } from '../../state/ducks/nav';

const drawerWidth = 240;

const useStyles = makeStyles(theme => ({
  appbar: {
    zIndex: theme.zIndex.drawer + 1,
    transition: theme.transitions.create(['width', 'margin'], {
      easing: theme.transitions.easing.sharp,
      duration: theme.transitions.duration.leavingScreen,
    }),
  },
  drawerOpen: {
    marginLeft: drawerWidth,
    width: `calc(100% - ${drawerWidth}px)`,
    transition: theme.transitions.create(['width', 'margin'], {
      easing: theme.transitions.easing.sharp,
      duration: theme.transitions.duration.enteringScreen,
    }),
  },
  menuButton: {
    marginRight: theme.spacing(2),
  },
  hide: {
    display: 'none',
  },
}));

export const PureAppBar = ({ isDrawerOpen, onDrawerOpen, title }) => {
  const classes = useStyles();
  return (
    <AppBar
      position='fixed'
      className={classNames(classes.appbar, {
        [classes.drawerOpen]: isDrawerOpen,
      })}
    >
      <Toolbar>
        <IconButton
          color='inherit'
          onClick={onDrawerOpen}
          edge='start'
          className={classNames(classes.menuButton, {
            [classes.hide]: isDrawerOpen,
          })}
        >
          <MenuIcon />
        </IconButton>
        <Typography color='inherit' variant='h6' noWrap>
          {title}
        </Typography>
      </Toolbar>
    </AppBar>
  );
};

// <IconButton color='inherit' onClick={onUserMenu}>
//   <AccountCircleIcon />
// </IconButton>

PureAppBar.propTypes = {
  title: PropTypes.string,
  isDrawerOpen: PropTypes.bool.isRequired,
  onDrawerOpen: PropTypes.func.isRequired,
};

PureAppBar.defaultProps = {
  title: 'UnderBudget',
};

const mapStateToProps = state => ({
  isDrawerOpen: state.nav.isDrawerOpen,
});

const mapDispatchToProps = dispatch => ({
  onDrawerOpen: dispatch(openDrawer()),
});

export default connect(mapStateToProps, mapDispatchToProps)(PureAppBar);
