import React from 'react';
import PropTypes from 'prop-types';
import { connect } from 'react-redux';
import { Formik, Form, Field } from 'formik';
import * as yup from 'yup';
import SubmitButton from '../SubmitButton/SubmitButton';
import TextInputField from '../TextInputField/TextInputField';
import { register } from '../../state/ducks/auth';

const schema = yup.object().shape({
  username: yup
    .string()
    .min(6)
    .max(128)
    .matches(/^[a-zA-Z0-9._]+$/, 'Only alphanumeric, ., or _ characters allowed')
    .required('Required'),
  email: yup
    .string()
    .email()
    .max(256)
    .required('Required'),
  password: yup
    .string()
    .min(12)
    .required('Required'),
});

export const PureUserRegistrationForm = ({ onRegister }) => (
  <Formik
    validationSchema={schema}
    onSubmit={onRegister}
  >
    {({ isSubmitting, isValid }) => (
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
        <SubmitButton text='Sign up' disabled={isSubmitting || !isValid} />
      </Form>
    )}
  </Formik>
);

PureUserRegistrationForm.propTypes = {
  onRegister: PropTypes.func.isRequired,
};

const mapDispatchToProps = dispatch => ({
  onRegister: user => dispatch(register(user)),
});

export default connect(null, mapDispatchToProps)(PureUserRegistrationForm);
