import React from 'react';
import PropTypes from 'prop-types';
import classNames from 'classnames';
import ChevronLeftIcon from '@material-ui/icons/ChevronLeft';
import Divider from '@material-ui/core/Divider';
import IconButton from '@material-ui/core/IconButton';
import MuiDrawer from '@material-ui/core/Drawer';
import withStyles from '@material-ui/core/styles/withStyles';

const drawerWidth = 240;

const styles = theme => ({
  drawer: {
    flexShrink: 0,
    whiteSpace: 'nowrap',
    width: drawerWidth,
  },

  drawerOpen: {
    width: drawerWidth,
    transition: theme.transitions.create('width', {
      easing: theme.transitions.easing.sharp,
      duration: theme.transitions.duration.enteringScreen,
    }),
  },

  drawerClosed: {
    overflowX: 'hidden',
    width: theme.spacing.unit * 7 + 1,
    transition: theme.transitions.create('width', {
      easing: theme.transitions.easing.sharp,
      duration: theme.transitions.duration.leavingScreen,
    }),
  },

  toolbar: {
    alignItems: 'center',
    display: 'flex',
    justifyContent: 'flex-end',
    padding: '0 8px',
    ...theme.mixins.toolbar,
  },
});

const Drawer = ({
  children,
  classes,
  onDrawerClose,
  open,
}) => (
  <MuiDrawer
    variant='permanent'
    className={classNames(classes.drawer, {
      [classes.drawerOpen]: open,
      [classes.drawerClosed]: !open,
    })}
    classes={{
      paper: classNames({
        [classes.drawerOpen]: open,
        [classes.drawerClosed]: !open,
      }),
    }}
    open={open}
  >
    <div className={classes.toolbar}>
      <IconButton onClick={onDrawerClose}>
        <ChevronLeftIcon />
      </IconButton>
    </div>
    <Divider />
    {children}
  </MuiDrawer>
);

Drawer.propTypes = {
  children: PropTypes.array.isRequired,
  classes: PropTypes.object.isRequired,
  open: PropTypes.bool.isRequired,
  onDrawerClose: PropTypes.func.isRequired,
};

export default withStyles(styles)(Drawer);
