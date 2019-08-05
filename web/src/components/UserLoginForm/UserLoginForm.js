import React from 'react';
import PropTypes from 'prop-types';
import { connect } from 'react-redux';
import { Formik, Form, Field } from 'formik';
import * as yup from 'yup';
import SubmitButton from '../SubmitButton/SubmitButton';
import TextInputField from '../TextInputField/TextInputField';
import { getIsLoginPending, login } from '../../state/ducks/auth';

const schema = yup.object().shape({
  username: yup.string().required('Required'),
  password: yup.string().required('Required'),
});

export const PureUserLoginForm = ({ isLoginPending, onLogin }) => (
  <Formik
    validateOnBlur={false}
    validationSchema={schema}
    onSubmit={onLogin}
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
          id='password'
          name='password'
          label='Password'
          type='password'
          autoComplete='current-password'
          component={TextInputField}
        />
        <SubmitButton text='Sign in' disabled={isLoginPending || !isValid} />
      </Form>
    )}
  </Formik>
);

PureUserLoginForm.propTypes = {
  isLoginPending: PropTypes.bool.isRequired,
  onLogin: PropTypes.func.isRequired,
};

const mapState = state => ({
  isLoginPending: getIsLoginPending(state),
});

const mapDispatch = dispatch => ({
  onLogin: user => dispatch(login(user)),
});

export default connect(mapState, mapDispatch)(PureUserLoginForm);
