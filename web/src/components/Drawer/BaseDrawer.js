import React from 'react';
import PropTypes from 'prop-types';
import classNames from 'classnames';
import { connect } from 'react-redux';
import ChevronLeftIcon from '@material-ui/icons/ChevronLeft';
import Divider from '@material-ui/core/Divider';
import IconButton from '@material-ui/core/IconButton';
import Drawer from '@material-ui/core/Drawer';
import useMediaQuery from '@material-ui/core/useMediaQuery';
import { makeStyles, useTheme } from '@material-ui/styles';
import { closeDrawer } from '../../state/ducks/nav';

const drawerWidth = 240;

const useStyles = makeStyles(theme => ({
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
    width: 0,
    transition: theme.transitions.create('width', {
      easing: theme.transitions.easing.sharp,
      duration: theme.transitions.duration.leavingScreen,
    }),
    [theme.breakpoints.up('sm')]: {
      width: theme.spacing(7) + 1,
    },
  },
  toolbar: {
    alignItems: 'center',
    display: 'flex',
    justifyContent: 'flex-end',
    padding: '0 8px',
    ...theme.mixins.toolbar,
  },
}));

export const PureBaseDrawer = ({ children, isDrawerOpen, onDrawerClose }) => {
  const classes = useStyles();
  const theme = useTheme();
  const mobile = useMediaQuery(theme.breakpoints.down('sm'));
  return (
    <Drawer
      variant={mobile ? 'temporary' : 'permanent'}
      className={classNames(classes.drawer, {
        [classes.drawerOpen]: isDrawerOpen,
        [classes.drawerClosed]: !isDrawerOpen,
      })}
      classes={{
        paper: classNames({
          [classes.drawerOpen]: isDrawerOpen,
          [classes.drawerClosed]: !isDrawerOpen,
        }),
      }}
      onClose={onDrawerClose}
      open={isDrawerOpen}
    >
      <div className={classes.toolbar}>
        <IconButton onClick={onDrawerClose}>
          <ChevronLeftIcon />
        </IconButton>
      </div>
      <Divider />
      {children}
    </Drawer>
  );
};

PureBaseDrawer.propTypes = {
  children: PropTypes.element.isRequired,
  isDrawerOpen: PropTypes.bool.isRequired,
  onDrawerClose: PropTypes.func.isRequired,
};

const mapStateToProps = state => ({
  isDrawerOpen: state.nav.isDrawerOpen,
});

const mapDispatchToProps = dispatch => ({
  onDrawerClose: () => dispatch(closeDrawer()),
});

export default connect(
  mapStateToProps,
  mapDispatchToProps,
)(PureBaseDrawer);
