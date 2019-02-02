import React from 'react'
import AppBar from '../components/AppBar/AppBar'
import Drawer from '../components/Drawer/AppDrawer'

class Dashboard extends React.Component {
  state = {
    drawerOpen: false
  }

  handleDrawerOpen = () => {
    console.log('handleDrawerOpen')
    this.setState({ drawerOpen: true })
  }
  
  handleDrawerClose = () => {
    console.log('handleDrawerClose')
    this.setState({ drawerOpen: false })
  }

  render() {
    return(
      <React.Fragment>
        <AppBar
          drawerOpen={this.state.drawerOpen}
          onDrawerOpen={this.handleDrawerOpen}
        />
        <Drawer open={this.state.drawerOpen} onDrawerClose={this.handleDrawerClose} />
        <h2>hi</h2>
      </React.Fragment>
    )
  }
}
        
export default Dashboard