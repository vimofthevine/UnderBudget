import React from 'react';
import PropTypes from 'prop-types';
import AppBar from '@material-ui/core/AppBar';
import Toolbar from '@material-ui/core/Toolbar';
import Typography from '@material-ui/core/Typography';
import { makeStyles } from '@material-ui/styles';

const useStyles = makeStyles(theme => ({
  appbar: {
    zIndex: theme.zIndex.drawer + 1,
  },
}));

const SimpleAppBar = ({ title }) => {
  const classes = useStyles();
  return (
    <AppBar
      position='fixed'
      className={classes.appbar}
    >
      <Toolbar>
        <Typography
          component='h1'
          variant='h6'
          color='inherit'
          noWrap
          style={{ flexGrow: 1 }}
        >
          {title}
        </Typography>
      </Toolbar>
    </AppBar>
  );
};

SimpleAppBar.propTypes = {
  title: PropTypes.string,
};

SimpleAppBar.defaultProps = {
  title: 'UnderBudget',
};

export default SimpleAppBar;
