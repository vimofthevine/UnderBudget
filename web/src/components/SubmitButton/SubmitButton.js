import React from 'react';
import PropTypes from 'prop-types';
import Button from '@material-ui/core/Button';
import { useTheme } from '@material-ui/styles';

const SubmitButton = ({ text, ...props }) => {
  const theme = useTheme();
  return (
    <Button
      type='submit'
      variant='contained'
      color='primary'
      fullWidth
      style={{ marginTop: theme.spacing(3) }}
      {...props}
    >
      {text}
    </Button>
  );
};

SubmitButton.propTypes = {
  text: PropTypes.string.isRequired,
};

export default SubmitButton;
