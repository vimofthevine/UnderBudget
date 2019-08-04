import React, { Fragment, useState } from 'react';
import { storiesOf } from '@storybook/react';
import { action } from '@storybook/addon-actions';
import { Provider } from 'react-redux';
import Register from './Register';

const makeStore = ({
  error = null,
  loggedIn = false,
}) => ({
  getState: () => ({
    auth: {
      error,
      loggedIn,
    },
  }),
  subscribe: () => 0,
  dispatch: action('dispatch'),
});

const DelayedPage = () => {
  const [show, setShow] = useState(false);
  const showPage = () => setShow(true);
  return (
    <Fragment>
      {show && <Register />}
      {!show && <button type='button' onClick={showPage}>Show</button>}
    </Fragment>
  );
};

storiesOf('User|Registration Page', module)
  .add('initial state', () => (
    <Provider store={makeStore({})}>
      <Register />
    </Provider>
  ))
  .add('already logged in', () => (
    <Provider store={makeStore({ loggedIn: true })}>
      <DelayedPage />
    </Provider>
  ))
  .add('register error', () => (
    <Provider store={makeStore({ error: 'Error registering user' })}>
      <Register />
    </Provider>
  ));
