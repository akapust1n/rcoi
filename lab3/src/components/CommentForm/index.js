import React, { Component } from "react";
import { Button, FormGroup, FormControl, ControlLabel, Col } from "react-bootstrap";
import _parseJSON from "../../HttpAssist"

export default class CommentFrom extends Component {
    constructor(props) {
        super(props);
        this.newsId = this.props.newsId;
        this.postComment = this.props.postComment;

        this.state = {
            body: ""
        };
        this.url = "https://kapust1n.ru:8079/comment";
    }

    validateForm() {
        return this.state.body.length > 0;
    }

    handleChange = event => {
        this.setState({
            [event.target.id]: event.target.value
        });
    }

    handleSubmit = event => {
        event.preventDefault();
        const body = this.state.body;
        const data = JSON.stringify({
            text: body,
            newsId: this.newsId
        });
        console.log(data);
        const authToken = localStorage.getItem("authtoken");
        console.log("TOKEN__", authToken);
        fetch(this.url, {
            method: 'POST',
            mode: 'cors',
            headers: {
                'Content-Type': 'application/json',
                'Authorization': 'Bearer ' + authToken
            },
            body: data,
        }).then(res => {
            if (res.status === 200) {
                this.setState({ body: "" });
                this.postComment();
            } else {
                throw new Error();
            }
        }).catch((error) => {
            alert("Cant create comment! You must log in");
        })
    }

    render() {
        return (
            <Col md={6} sm={6} >
                <div className="CreateComment">
                    <form>
                        <FormGroup controlId="body" >
                            <FormControl
                                value={this.state.body}
                                onChange={this.handleChange}
                                componentClass="textarea"
                                className="textBody"
                                rows="2"
                                placeholder="Your comment"
                            />
                        </FormGroup>
                        <Button
                            block
                            bsSize="sm"
                            disabled={!this.validateForm()}
                            onClick={this.handleSubmit}
                        >
                            Comment
            </Button>
                    </form>
                </div>
            </Col>
        );
    }
}