import React, { Fragment, useState } from 'react';
import { storiesOf } from '@storybook/react';
import { action } from '@storybook/addon-actions';
import { Provider } from 'react-redux';
import Login from './Login';

const makeStore = ({
  loggedIn = false,
  username = null,
}) => ({
  getState: () => ({
    auth: {
      loggedIn,
      username,
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
      {show && <Login />}
      {!show && <button type='button' onClick={showPage}>Show</button>}
    </Fragment>
  );
};

storiesOf('User|Login Page', module)
  .add('initial state', () => (
    <Provider store={makeStore({})}>
      <Login />
    </Provider>
  ))
  .add('already logged in', () => (
    <Provider store={makeStore({ loggedIn: true, username: 'testuser' })}>
      <DelayedPage />
    </Provider>
  ));
