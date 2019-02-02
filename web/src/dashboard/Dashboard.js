import React from 'react'
import AppBar from '../components/AppBar/AppBar'
import Drawer from '../components/Drawer/AppDrawer'
import UserMenu from '../components/UserMenu/UserMenu'

class Dashboard extends React.Component {
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
    return(
      <React.Fragment>
        <AppBar
          drawerOpen={this.state.drawerOpen}
          onDrawerOpen={this.handleDrawerOpen}
          onUserMenu={this.handleUserMenuOpen}
        />
        <Drawer open={this.state.drawerOpen} onDrawerClose={this.handleDrawerClose} />
        <UserMenu
          history={this.props.history}
          anchor={this.state.userMenuAnchor}
          onClose={this.handleUserMenuClose}
        />
        <h2>hi</h2>
      </React.Fragment>
    )
  }
}
        
export default Dashboard