import React from 'react';
import PropTypes from 'prop-types';
import Menu from '@material-ui/core/Menu';
import MenuItem from '@material-ui/core/MenuItem';
import AuthService from '../../auth/AuthService';

class UserMenu extends React.Component {
  handleLogout = () => {
    const auth = new AuthService();
    auth.logout();
    const { history } = this.props;
    history.replace('/login');
  }

  render() {
    const { anchor, onClose } = this.props;
    const open = Boolean(anchor);
    return (
      <Menu
        anchorEl={anchor}
        anchorOrigin={{
          vertical: 'top',
          horizontal: 'right',
        }}
        transformOrigin={{
          vertical: 'top',
          horizontal: 'right',
        }}
        open={open}
        onClose={onClose}
      >
        <MenuItem>Profile</MenuItem>
        <MenuItem onClick={this.handleLogout}>Logout</MenuItem>
      </Menu>
    );
  }
}

UserMenu.propTypes = {
  history: PropTypes.object.isRequired,
  anchor: PropTypes.object.isRequired,
  onClose: PropTypes.func.isRequired,
};

export default UserMenu;
