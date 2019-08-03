import React from 'react';
import PropTypes from 'prop-types';
import { connect } from 'react-redux';
import { Formik, Form, Field } from 'formik';
import * as yup from 'yup';
import SubmitButton from '../SubmitButton/SubmitButton';
import TextInputField from '../TextInputField/TextInputField';
import { login } from '../../state/ducks/auth';

const schema = yup.object().shape({
  username: yup.string().required('Required'),
  password: yup.string().required('Required'),
});

export const PureUserLoginForm = ({ onLogin }) => (
  <Formik
    validationSchema={schema}
    onSubmit={onLogin}
  >
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
        id='password'
        name='password'
        label='Password'
        type='password'
        autoComplete='current-password'
        component={TextInputField}
      />
      <SubmitButton text='Sign in' />
    </Form>
  </Formik>
);

PureUserLoginForm.propTypes = {
  onLogin: PropTypes.func.isRequired,
};

const mapDispatchToProps = dispatch => ({
  onLogin: user => dispatch(login(user)),
});

export default connect(null, mapDispatchToProps)(PureUserLoginForm);
