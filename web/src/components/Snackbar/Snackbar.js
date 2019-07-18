import React from 'react';
import { Snackbar as MuiSnackbar } from '@material-ui/core';
import { makeStyles } from '@material-ui/styles';

const useStyles = makeStyles(theme => ({
  snackbar: {
    [theme.breakpoints.down('sm')]: {
      bottom: 90,
    },
  },
}));

const Snackbar = (props) => {
  const classes = useStyles();
  return (
    <MuiSnackbar
      autoHideDuration={3000}
      className={classes.snackbar}
      {...props}
    />
  );
};

export default Snackbar;
