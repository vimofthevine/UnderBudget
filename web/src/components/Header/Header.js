import React, { Fragment, useState } from 'react';
import AppBar from '../AppBar/AppBar';
import Drawer from '../Drawer/Drawer';
import UserMenu from '../UserMenu/UserMenu';

const Header = () => {
  const [userMenuAnchor, setUserMenuAnchor] = useState(null);
  const openUserMenu = event => setUserMenuAnchor(event.currentTarget);
  const closeUserMenu = () => setUserMenuAnchor(null);
  return (
    <Fragment>
      <AppBar onOpenUserMenu={openUserMenu} />
      <UserMenu anchor={userMenuAnchor} onClose={closeUserMenu} />
      <Drawer />
    </Fragment>
  );
};
export default Header;
