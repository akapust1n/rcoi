import React, { Component } from "react";
import { Navbar, NavItem, Nav, Grid, Row, Col } from "react-bootstrap";
import Login from "./components/Login"
import Register from "./components/Register"
import CreateNews from "./components/CreateNews"


const ROUTES = [
  { name: "login" },
  { name: "register" },
  { name: "create news" },

]


class App extends Component {
  constructor() {
    super();
    this.state = {
      route: 0
    };
  }
  render() {
    const route = this.state.route;
    let mainPart;
    switch (route) {
      case 0:
        mainPart = <Login />
        break;
      case 1:
        mainPart = <Register />
        break;
      case 2:
        mainPart = <CreateNews />
        break;
      default:
        mainPart = <Login />

    }

    return (< div >
      <Navbar>
        <Navbar.Header>
          <Navbar.Brand>
            News Application
        </Navbar.Brand>
        </Navbar.Header>
      </Navbar>
      <Grid>
        <Row>
          <Col md={4} sm={4}>
            <h3>Select action</h3>
            <Nav
              bsStyle="pills"
              stacked
              activeKey={route}
              onSelect={index => {
                this.setState({ route: index });
              }}
            >

              {ROUTES.map((rout, index) => (
                <NavItem key={index} eventKey={index}>{rout.name}</NavItem>
              ))}
            </Nav>
          </Col>
          <Col md={8} sm={8}>
            {mainPart}

          </Col>
        </Row>
      </Grid>
    </div >
    );
  }
}

export default App;