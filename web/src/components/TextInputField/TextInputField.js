import React from 'react';
import PropTypes from 'prop-types';
import TextField from '@material-ui/core/TextField';

const TextInputField = ({ field, form, helperText, ...props }) => (
  <TextField
    margin='normal'
    required
    fullWidth
    error={form.errors[field.name]}
    helperText={form.errors[field.name] || helperText}
    {...field}
    {...props}
  />
);

TextInputField.propTypes = {
  field: PropTypes.object,
  form: PropTypes.object,
  helperText: PropTypes.string,
};

TextInputField.defaultProps = {
  field: { name: '' },
  form: { errors: {} },
  helperText: null,
};

export default TextInputField;
