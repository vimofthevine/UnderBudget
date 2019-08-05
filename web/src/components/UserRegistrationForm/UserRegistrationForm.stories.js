import React from 'react';
import { storiesOf } from '@storybook/react';
import { action } from '@storybook/addon-actions';
import { PureUserRegistrationForm } from './UserRegistrationForm';

storiesOf('User|UserRegistrationForm', module)
  .add('initial state', () => (
    <PureUserRegistrationForm onRegister={action('register')} />
  ))
  .add('submission pending', () => (
    <PureUserRegistrationForm isRegisterPending onRegister={action('register')} />
  ));
