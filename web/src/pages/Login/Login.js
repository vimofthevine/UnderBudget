import React from 'react';
import PropTypes from 'prop-types';
import { connect } from 'react-redux';
import { push } from 'connected-react-router';
import { Button, Snackbar, Typography } from '@material-ui/core';
import { makeStyles } from '@material-ui/styles';
import LockOutlinedIcon from '@material-ui/icons/LockOutlined';
import NoNavPage from '../../components/NoNavPage/NoNavPage';
import UserLoginForm from '../../components/UserLoginForm/UserLoginForm';
import useAuthRequired from '../../hooks/useAuthRequired';
import { dismissAuthError, getAuthError } from '../../state/ducks/auth';

const useStyles = makeStyles(theme => ({
  signup: {
    marginTop: theme.spacing(3),
    width: '100%',
  },
}));

const Login = ({ authError, onDismissError, onSignUp }) => {
  useAuthRequired(false, '/');
  const classes = useStyles();

  return (
    <NoNavPage avatar={<LockOutlinedIcon />} title='Sign in'>
      <UserLoginForm />

      <div className={classes.signup}>
        <Typography variant='body1' align='right'>
          Don&lsquo;t have an account?
          {' '}
          <Button color='secondary' disableRipple onClick={onSignUp}>
            Sign up!
          </Button>
        </Typography>
      </div>

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
  authError: PropTypes.string,
  onDismissError: PropTypes.func.isRequired,
  onSignUp: PropTypes.func.isRequired,
};

Login.defaultProps = {
  authError: null,
};

const mapState = state => ({
  authError: getAuthError(state),
});

const mapDispatch = dispatch => ({
  onDismissError: () => dispatch(dismissAuthError()),
  onSignUp: () => dispatch(push('/register')),
});

export default connect(
  mapState,
  mapDispatch,
)(Login);
