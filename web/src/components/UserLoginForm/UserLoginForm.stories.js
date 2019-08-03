import React from 'react';
import { storiesOf } from '@storybook/react';
import { action } from '@storybook/addon-actions';
import { PureUserLoginForm } from './UserLoginForm';

storiesOf('User|UserLoginForm', module)
  .add('default', () => (
    <PureUserLoginForm onLogin={action('login')} />
  ));
