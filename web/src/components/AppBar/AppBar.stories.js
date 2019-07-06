import React, { Fragment, useState } from 'react';
import { storiesOf } from '@storybook/react';
import { action } from '@storybook/addon-actions';
import { PureAppBar } from './AppBar';

const openDrawer = action('open drawer');

const AppBar = ({ title = undefined }) => {
  const [drawerOpen, setDrawerOpen] = useState(false);
  return (
    <Fragment>
      <PureAppBar
        title={title}
        isDrawerOpen={drawerOpen}
        onDrawerOpen={openDrawer}
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

storiesOf('Layout|AppBar', module)
  .add('full', () => (
    <AppBar />
  ))
  .add('full with title', () => (
    <AppBar title='App Title' />
  ));
