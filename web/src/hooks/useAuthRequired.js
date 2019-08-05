import { useEffect } from 'react';
import { useDispatch, useSelector } from 'react-redux';
import { push } from 'connected-react-router';
import { getIsLoggedIn } from '../state/ducks/auth';

const useAuthRequired = (required, redirectTo) => {
  const loggedIn = useSelector(getIsLoggedIn);
  const dispatch = useDispatch();
  useEffect(() => {
    if ((loggedIn && !required) || (!loggedIn && required)) {
      dispatch(push(redirectTo));
    }
  }, [required, redirectTo, loggedIn, dispatch]);
};

export default useAuthRequired;
