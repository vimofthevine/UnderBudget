import React from 'react';
import { storiesOf } from '@storybook/react';
import { Formik } from 'formik';
import UserLoginForm from './UserLoginForm';

storiesOf('User|UserLoginForm', module)
  .add('default', () => (
    <Formik>
      <UserLoginForm />
    </Formik>
  ));
