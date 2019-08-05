import React from 'react';
import { storiesOf } from '@storybook/react';
import { action } from '@storybook/addon-actions';
import { PureUserLoginForm } from './UserLoginForm';

storiesOf('User|UserLoginForm', module)
  .add('initial state', () => (
    <PureUserLoginForm onLogin={action('login')} />
  ))
  .add('submission pending', () => (
    <PureUserLoginForm isLoginPending onLogin={action('login')} />
  ));
