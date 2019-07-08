import React, { Fragment, useState } from 'react';
import { storiesOf } from '@storybook/react';
import { action } from '@storybook/addon-actions';
import { PureAppBar } from './AppBar';

const openDrawer = action('open drawer');
const openUserMenu = action('open user menu');

// eslint-disable-next-line react/prop-types
const AppBar = ({ title = undefined }) => {
  const [drawerOpen, setDrawerOpen] = useState(false);
  return (
    <Fragment>
      <PureAppBar
        title={title}
        isDrawerOpen={drawerOpen}
        onDrawerOpen={openDrawer}
        onOpenUserMenu={openUserMenu}
      />
      <button
        type='button'
        onClick={() => setDrawerOpen(!drawerOpen)}
        style={{ marginTop: '100px' }}
      >
        Toggle drawer
      </button>
    </Fragment>
  );
};

storiesOf('Layout|Header/AppBar', module)
  .add('full', () => (
    <AppBar />
  ))
  .add('full with title', () => (
    <AppBar title='App Title' />
  ));
