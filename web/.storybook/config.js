import React, { Fragment } from 'react';
import { addDecorator, configure } from '@storybook/react';
import CssBaseline from '@material-ui/core/CssBaseline';
import { blueGrey, green } from '@material-ui/core/colors';
import { createMuiTheme } from '@material-ui/core/styles';
import { ThemeProvider } from '@material-ui/styles';

const theme = createMuiTheme({
  palette: {
    primary: {
      main: green[600],
    },
    secondary: blueGrey,
  },
});

addDecorator(storyFn => (
  <Fragment>
    <CssBaseline />
    <ThemeProvider theme={theme}>
      {storyFn()}
    </ThemeProvider>
  </Fragment>
));

// automatically import all files ending in *.stories.js
const req = require.context('../src', true, /\.stories\.js$/);
function loadStories() {
  req.keys().forEach(filename => req(filename));
}

configure(loadStories, module);
