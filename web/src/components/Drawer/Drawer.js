import React from 'react'
import PropTypes from 'prop-types'
import classNames from 'classnames'
import ChevronLeftIcon from '@material-ui/icons/ChevronLeft'
import Divider from '@material-ui/core/Divider'
import IconButton from '@material-ui/core/IconButton'
import MuiDrawer from '@material-ui/core/Drawer'
import withStyles from '@material-ui/core/styles/withStyles'

const drawerWidth = 240

const styles = theme => ({
  drawer: {
    flexShrink: 0,
    whiteSpace: 'nowrap',
    width: drawerWidth
  },
    
  drawerOpen: {
    width: drawerWidth,
    transition: theme.transitions.create('width', {
      easing: theme.transitions.easing.sharp,
      duration: theme.transitions.duration.enteringScreen
    })
  },
    
  drawerClosed: {
    overflowX: 'hidden',
    width: theme.spacing.unit * 7 + 1,
    transition: theme.transitions.create('width', {
      easing: theme.transitions.easing.sharp,
      duration: theme.transitions.duration.leavingScreen
    })
  },
    
  toolbar: {
    alignItems: 'center',
    display: 'flex',
    justifyContent: 'flex-end',
    padding: '0 8px',
    ...theme.mixins.toolbar,
  }
})

class Drawer extends React.Component {
  render() {
    const { classes } = this.props
    return (
      <MuiDrawer variant="permanent"
        className={classNames(classes.drawer, {
          [classes.drawerOpen]: this.props.open,
          [classes.drawerClosed]: !this.props.open
        })}
        classes={{
          paper: classNames({
            [classes.drawerOpen]: this.props.open,
            [classes.drawerClosed]: !this.props.open
          })
        }}
        open={this.props.open}
      >
        <div className={classes.toolbar}>
          <IconButton onClick={this.props.onDrawerClose}>
            <ChevronLeftIcon />
          </IconButton>
        </div>
        <Divider />
        {this.props.children}
      </MuiDrawer>
    )
  }
}

Drawer.propTypes = {
  open: PropTypes.bool.isRequired,
  onDrawerClose: PropTypes.func.isRequired
}

export default withStyles(styles)(Drawer)