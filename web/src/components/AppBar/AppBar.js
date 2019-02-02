import React from 'react'
import PropTypes from 'prop-types'
import classNames from 'classnames'
import AccountCircleIcon from '@material-ui/icons/AccountCircle'
import MuiAppBar from '@material-ui/core/AppBar'
import IconButton from '@material-ui/core/IconButton'
import MenuIcon from '@material-ui/icons/Menu'
import Toolbar from '@material-ui/core/Toolbar'
import Typography from '@material-ui/core/Typography'
import withStyles from '@material-ui/core/styles/withStyles'

const drawerWidth = 240

const styles = theme => ({
  appbar: {
    zIndex: theme.zIndex.drawer + 1,
    transition: theme.transitions.create(['width', 'margin'], {
      easing: theme.transitions.easing.sharp,
      duration: theme.transitions.duration.leavingScreen
    })
  },
    
  drawerOpen: {
    marginLeft: drawerWidth,
    width: `calc(100% - ${drawerWidth}px)`,
    transition: theme.transitions.create(['width', 'margin'], {
      easing: theme.transitions.easing.sharp,
      duration: theme.transitions.duration.enteringScreen
    })
  },
    
  menuButton: {
    marginLeft: 12,
    marginRight: 36
  },
    
  hide: {
    display: 'none'
  },
    
  grow: {
    flexGrow: 1
  }
})

class AppBar extends React.Component {
  render() {
    const { classes, onDrawerOpen, onUserMenu } = this.props;
    return(
      <MuiAppBar
        position="fixed"
        className={classNames(classes.appbar, {
          [classes.drawerOpen]: this.props.drawerOpen
        })}
      >
        <Toolbar disableGutters={onDrawerOpen && !this.props.drawerOpen}>
          {onDrawerOpen &&
            <IconButton
              color="inherit"
              onClick={onDrawerOpen}
              className={classNames(classes.menuButton, {
                [classes.hide]: this.props.drawerOpen
              })}
            >
              <MenuIcon />
            </IconButton>
          }
          <Typography component="h1" variant="h6" color="inherit" noWrap className={classes.grow}>
            {this.props.title}
          </Typography>
          {onUserMenu &&
            <IconButton
              color="inherit"
              onClick={onUserMenu}
            >
              <AccountCircleIcon />
            </IconButton>
          }
        </Toolbar>
      </MuiAppBar>
    )
  }
}

AppBar.defaultProps = {
  title: "UnderBudget",
  drawerOpen: false
}

AppBar.propTypes = {
  title: PropTypes.string.isRequired,
  classes: PropTypes.object.isRequired,
  drawerOpen: PropTypes.bool,
  onDrawerOpen: PropTypes.func,
  onUserMenu: PropTypes.func
}

export default withStyles(styles)(AppBar)