import React from 'react';
import { Provider } from 'react-redux';
import { storiesOf } from '@storybook/react';
import { action } from '@storybook/addon-actions';
import Paper from '@material-ui/core/Paper';
import FullNavPage from './FullNavPage';

const createStore = isDrawerOpen => ({
  getState: () => ({
    nav: {
      isDrawerOpen,
    },
  }),
  subscribe: () => 0,
  dispatch: action('dispatch'),
});

storiesOf('Layout|FullNavPage', module)
  .add('drawer closed', () => (
    <Provider store={createStore(false)}>
      <FullNavPage>
        <Paper>
          <h1>Page title</h1>
          <p>Page content</p>
        </Paper>
      </FullNavPage>
    </Provider>
  ))
  .add('drawer open', () => (
    <Provider store={createStore(true)}>
      <FullNavPage>
          <h1>Page title</h1>
          <p>Page content</p>
      </FullNavPage>
    </Provider>
  ));
