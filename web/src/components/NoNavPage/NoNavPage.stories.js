import React from 'react';
import { storiesOf } from '@storybook/react';
import AccountCircleIcon from '@material-ui/icons/AccountCircle';
import LockOutlinedIcon from '@material-ui/icons/LockOutlined';
import NoNavPage from './NoNavPage';

storiesOf('Layout|NoNavPage', module)
  .add('Login', () => (
    <NoNavPage
      avatar={<LockOutlinedIcon />}
      title='Sign in'
    >
      Login form
    </NoNavPage>
  ))
  .add('Register', () => (
    <NoNavPage
      avatar={<AccountCircleIcon />}
      title='Sign up'
    >
      Registration form
    </NoNavPage>
  ));
