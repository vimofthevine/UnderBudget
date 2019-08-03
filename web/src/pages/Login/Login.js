import React from 'react';
import LockOutlinedIcon from '@material-ui/icons/LockOutlined';
import NoNavPage from '../../components/NoNavPage/NoNavPage';
import UserLoginForm from '../../components/UserLoginForm/UserLoginForm';
import useAuthRequired from '../../hooks/useAuthRequired';

const Login = () => {
  useAuthRequired(false, '/');
  return (
    <NoNavPage
      avatar={<LockOutlinedIcon />}
      title='Sign in'
    >
      <UserLoginForm />
    </NoNavPage>
  );
};

export default Login;
