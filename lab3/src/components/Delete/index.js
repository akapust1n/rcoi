import React, { Component } from "react";
import { Button, FormGroup, FormControl, ControlLabel, Col } from "react-bootstrap";
import { _parseJSON } from "../../HttpAssist"


export default class Delete extends Component {
    constructor(props) {
        super(props);
        this.url = "http://kapust1n.ru:8080/delete";
    }



    deleteAccount = event => {
        const authToken = localStorage.getItem("authtoken");


        fetch(this.url, {
            method: 'DELETE',
            mode: 'cors',
            headers: {
                'Content-Type': 'application/json',
                'Authorization': 'Bearer ' + authToken
            },
        }).then(async res => {
            if (res.status === 200) {
                alert("account is deleted");
                return _parseJSON(res);
            } else {
                const response = await _parseJSON(res);
                alert(response["error"]);
                throw new Error();
            }
        }).catch((error) => {
        })
    }

    render() {
        return (
            <Col md={4} sm={4} >
                <div className="Delete">
                    <Button
                        block
                        bsSize="large"
                        onClick={this.deleteAccount}
                    >
                        Delete Account
            </Button>
                </div>
            </Col>
        );
    }
}