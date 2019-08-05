import React from 'react';
import PropTypes from 'prop-types';
import { connect } from 'react-redux';
import { Snackbar } from '@material-ui/core';
import AccountCircleIcon from '@material-ui/icons/AccountCircle';
import NoNavPage from '../../components/NoNavPage/NoNavPage';
import UserRegistrationForm from '../../components/UserRegistrationForm/UserRegistrationForm';
import useAuthRequired from '../../hooks/useAuthRequired';
import { dismissAuthError, getAuthError } from '../../state/ducks/auth';

const Register = ({ authError, onDismissError }) => {
  useAuthRequired(false, '/');
  return (
    <NoNavPage avatar={<AccountCircleIcon />} title='Sign up'>
      <UserRegistrationForm />

      <Snackbar
        autoHideDuration={3000}
        message={authError}
        onClose={onDismissError}
        open={authError}
      />
    </NoNavPage>
  );
};

Register.propTypes = {
  authError: PropTypes.string,
  onDismissError: PropTypes.func.isRequired,
};

Register.defaultProps = {
  authError: null,
};

const mapState = state => ({
  authError: getAuthError(state),
});

const mapDispatch = dispatch => ({
  onDismissError: () => dispatch(dismissAuthError()),
});

export default connect(mapState, mapDispatch)(Register);