import React from 'react';
import { Provider } from 'react-redux';
import { storiesOf } from '@storybook/react';
import { action } from '@storybook/addon-actions';
import Header from './Header';

const createStore = isDrawerOpen => ({
  getState: () => ({
    nav: {
      isDrawerOpen,
    },
  }),
  subscribe: () => 0,
  dispatch: action('dispatch'),
});

storiesOf('Layout|Header', module)
  .add('drawer closed', () => (
    <Provider store={createStore(false)}>
      <Header />
    </Provider>
  ))
  .add('drawer open', () => (
    <Provider store={createStore(true)}>
      <Header />
    </Provider>
  ));
