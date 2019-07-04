import React from 'react';
import { storiesOf } from '@storybook/react';
import AccountCircleIcon from '@material-ui/icons/AccountCircle';
import LockOutlinedIcon from '@material-ui/icons/LockOutlined';
import SingleElementPage from './SingleElementPage';

storiesOf('Layout|SingleElementPage', module)
  .add('Login', () => (
    <SingleElementPage
      avatar={<LockOutlinedIcon />}
      title='Sign in'
    >
      Login form
    </SingleElementPage>
  ))
  .add('Register', () => (
    <SingleElementPage
      avatar={<AccountCircleIcon />}
      title='Sign up'
    >
      Registration form
    </SingleElementPage>
  ));
