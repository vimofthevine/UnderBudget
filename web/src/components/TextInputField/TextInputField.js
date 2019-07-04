import React from 'react';
import PropTypes from 'prop-types';
import TextField from '@material-ui/core/TextField';

const TextInputField = ({ field, ...props }) => (
  <TextField
    margin='normal'
    required
    fullWidth
    {...field}
    {...props}
  />
);

TextInputField.propTypes = {
  field: PropTypes.object,
};

TextInputField.defaultProps = {
  field: null,
};

export default TextInputField;
