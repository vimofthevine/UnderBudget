import React from 'react'
import PropTypes from 'prop-types'
import Menu from '@material-ui/core/Menu'
import MenuItem from '@material-ui/core/MenuItem'
import AuthService from '../../auth/AuthService'

class UserMenu extends React.Component {
  handleLogout = () => {
    const auth = new AuthService()
    auth.logout()
    this.props.history.replace('/login')
  }
  
  render() {
    const open = Boolean(this.props.anchor)
    return (
      <Menu
        anchorEl={this.props.anchor}
        anchorOrigin={{
          vertical: 'top',
          horizontal: 'right'
        }}
        transformOrigin={{
          vertical: 'top',
          horizontal: 'right'
        }}
        open={open}
        onClose={this.props.onClose}
      >
        <MenuItem>Profile</MenuItem>
        <MenuItem onClick={this.handleLogout}>Logout</MenuItem>
      </Menu>
    )
  }
}

UserMenu.propTypes = {
  history: PropTypes.object,
  anchor: PropTypes.object,
  onClose: PropTypes.func
}

export default UserMenu