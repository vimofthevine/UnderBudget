import React, { Fragment, useState } from 'react';
import { storiesOf } from '@storybook/react';
import { action } from '@storybook/addon-actions';
import UserMenu from './UserMenu';

const logout = action('logout');
const profile = action('profile');

const UserMenuDemo = () => {
  const [anchor, setAnchor] = useState(null);
  const openMenu = event => setAnchor(event.currentTarget);
  const closeMenu = () => setAnchor(null);
  return (
    <Fragment>
      <UserMenu
        anchor={anchor}
        onClose={closeMenu}
        onLogout={logout}
        onProfile={profile}
      />
      <button
        type='button'
        onClick={openMenu}
      >
        Open menu
      </button>
    </Fragment>
  );
};

storiesOf('Layout|Header/UserMenu', module)
  .add('default', () => (
    <UserMenuDemo />
  ));
