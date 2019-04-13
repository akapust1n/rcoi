import React, { Component } from "react";
import { Button, FormGroup, FormControl, ControlLabel, Col } from "react-bootstrap";
import { _parseJSON } from "../../HttpAssist"
import './style.css'


export default class Login extends Component {
    constructor(props) {
        super(props);

        this.state = {
            login: "",
            password: ""
        };
        this.url = "https://kapust1n.ru:8079/login";
    }

    validateForm() {
        return this.state.login.length > 0 && this.state.password.length > 0;
    }

    handleChange = event => {
        this.setState({
            [event.target.id]: event.target.value
        });
    }

    handleSubmit = event => {
        event.preventDefault();
        const login = this.state.login;
        const password = this.state.password;
        const data = JSON.stringify({
            name: login,
            password: password,
        });

        fetch(this.url, {
            method: 'POST',
            mode: 'cors',
            headers: {
                'Content-Type': 'application/json',
            },
            body: data,
        }).then(async res => {
            if (res.status === 200) {
                const response = await _parseJSON(res);
                if (response.hasOwnProperty("error")) {
                    alert(response["error"]);
                    throw new Error();
                }
                else {
                    return response;
                }
            } else {
                const response = await _parseJSON(res);
                alert(response["error"]);
                throw new Error();
            }
        }).then(json => {
            console.log("login json", json);
            localStorage.setItem("authtoken", json["authtoken"]);
            alert("login is ok");
        })
            .catch((error) => {
            })
    }

    render() {
        return (
            <Col md={4} sm={4} >
                <div className="Login">
                    <form onSubmit={this.handleSubmit}>
                        <FormGroup controlId="login" >
                            <ControlLabel>login</ControlLabel>
                            <FormControl
                                autoFocus
                                type="login"
                                value={this.state.login}
                                onChange={this.handleChange}
                            />
                        </FormGroup>
                        <FormGroup controlId="password" >
                            <ControlLabel>Password</ControlLabel>
                            <FormControl
                                value={this.state.password}
                                onChange={this.handleChange}
                                type="password"
                            />
                        </FormGroup>
                        <Button
                            block
                            bsSize="large"
                            disabled={!this.validateForm()}
                            type="submit"
                        >
                            Login
            </Button>
                    </form>
                </div>
            </Col>
        );
    }
}