import React from 'react';
import { storiesOf } from '@storybook/react';
import { Formik } from 'formik';
import UserRegistrationForm from './UserRegistrationForm';

storiesOf('User|UserRegistrationForm', module)
  .add('default', () => (
    <Formik>
      <UserRegistrationForm />
    </Formik>
  ));
