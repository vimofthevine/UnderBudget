import React, { useState } from 'react';
import { storiesOf } from '@storybook/react';
import DashboardIcon from '@material-ui/icons/Dashboard';
import List from '@material-ui/core/List';
import ListItem from '@material-ui/core/ListItem';
import ListItemIcon from '@material-ui/core/ListItemIcon';
import ListItemText from '@material-ui/core/ListItemText';
import { PureBaseDrawer } from './BaseDrawer';

const Drawer = () => {
  const [drawerOpen, setDrawerOpen] = useState(false);
  const handleClose = () => setDrawerOpen(false);
  return (
    <div style={{ display: 'flex' }}>
      <PureBaseDrawer
        isDrawerOpen={drawerOpen}
        onDrawerClose={handleClose}
      >
        <List>
          <ListItem button>
            <ListItemIcon>
              <DashboardIcon />
            </ListItemIcon>
            <ListItemText primary='Dashboard' />
          </ListItem>
        </List>
      </PureBaseDrawer>
      <main>
        <p>
          Lorem ipsum dolor sit amet
        </p>
        <button
          type='button'
          onClick={() => setDrawerOpen(!drawerOpen)}
          style={{ marginLeft: '250px' }}
        >
          Toggle drawer
        </button>
      </main>
    </div>
  );
};

storiesOf('Layout|Header/Drawer', module)
  .add('base', () => (
    <Drawer />
  ));
