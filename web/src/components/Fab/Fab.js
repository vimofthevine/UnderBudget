import React from 'react';
import PropTypes from 'prop-types';
import { Fab as MuiFab } from '@material-ui/core';
import { makeStyles } from '@material-ui/styles';

const useStyles = makeStyles(theme => ({
  fab: {
    position: 'absolute',
    bottom: theme.spacing(2),
    right: theme.spacing(2),
  },
}));

const Fab = ({ children, onClick, ...props }) => {
  const classes = useStyles();
  return (
    <MuiFab
      color='primary'
      onClick={onClick}
      className={classes.fab}
      {...props}
    >
      {children}
    </MuiFab>
  );
};

Fab.propTypes = {
  children: PropTypes.element.isRequired,
  onClick: PropTypes.func.isRequired,
}

export default Fab;
