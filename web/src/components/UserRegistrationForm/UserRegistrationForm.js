import React from 'react';
import { Form, Field } from 'formik';
import SubmitButton from '../SubmitButton/SubmitButton';
import TextInputField from '../TextInputField/TextInputField';

const UserRegisrationForm = () => (
  <Form>
    <Field
      id='username'
      name='username'
      label='Username'
      autoComplete='username'
      autoFocus
      component={TextInputField}
    />
    <Field
      id='email'
      name='email'
      label='Email Address'
      autoComplete='email'
      component={TextInputField}
    />
    <Field
      id='password'
      name='password'
      label='Password'
      type='password'
      autoComplete='current-password'
      component={TextInputField}
    />
    <SubmitButton text='Sign up' />
  </Form>
);

export default UserRegisrationForm;
