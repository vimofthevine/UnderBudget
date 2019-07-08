import React from 'react';
import PropTypes from 'prop-types';
import Menu from '@material-ui/core/Menu';
import MenuItem from '@material-ui/core/MenuItem';

const UserMenu = ({ anchor, onClose, onLogout, onProfile }) => {
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
      <MenuItem onClick={onProfile}>Profile</MenuItem>
      <MenuItem onClick={onLogout}>Logout</MenuItem>
    </Menu>
  );
};

UserMenu.propTypes = {
  anchor: PropTypes.object.isRequired,
  onClose: PropTypes.func.isRequired,
  onLogout: PropTypes.func.isRequired,
  onProfile: PropTypes.func.isRequired,
};

export default UserMenu;
