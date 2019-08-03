import React from 'react';
import PropTypes from 'prop-types';
import { connect } from 'react-redux';
import { Snackbar } from '@material-ui/core';
import LockOutlinedIcon from '@material-ui/icons/LockOutlined';
import NoNavPage from '../../components/NoNavPage/NoNavPage';
import UserLoginForm from '../../components/UserLoginForm/UserLoginForm';
import useAuthRequired from '../../hooks/useAuthRequired';
import { dismissAuthError } from '../../state/ducks/auth';

const Login = ({ authError, onDismissError }) => {
  useAuthRequired(false, '/');
  return (
    <NoNavPage
      avatar={<LockOutlinedIcon />}
      title='Sign in'
    >
      <UserLoginForm />
      <Snackbar
        autoHideDuration={3000}
        message={authError}
        onClose={onDismissError}
        open={authError}
      />
    </NoNavPage>
  );
};

Login.propTypes = {
  authError: PropTypes.string.isRequired,
  onDismissError: PropTypes.func.isRequired,
};

const mapState = state => ({
  authError: state.auth.error,
});

const mapDispatch = dispatch => ({
  onDismissError: () => dispatch(dismissAuthError()),
});

export default connect(mapState, mapDispatch)(Login);
