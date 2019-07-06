import React from 'react';
import { Provider } from 'react-redux';
import { storiesOf } from '@storybook/react';
import { action } from '@storybook/addon-actions';
import Drawer from './Drawer';

const store = {
  getState: () => ({
    nav: {
      isDrawerOpen: true,
    },
  }),
  subscribe: () => 0,
  dispatch: action('dispatch'),
};

storiesOf('Layout|Drawer', module)
  .addDecorator(storyFn => <Provider store={store}>{storyFn()}</Provider>)
  .add('default app drawer', () => (
    <Drawer />
  ));
