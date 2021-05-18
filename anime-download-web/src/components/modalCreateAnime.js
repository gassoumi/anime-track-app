import React, { useState } from 'react';
import { Modal, Button } from 'react-bootstrap'
import axios from "axios";
import { toast } from 'react-toastify';
const API = "/api/anime";
export default function ModalCreateAnime({ show, handleClose }) {

    const [name, setName] = useState("");
    const [searchKey, setSearchKey] = useState("");

    const handleSubmit = () => {
        console.log(name, searchKey);
        if (name) {
            let url = `${API}/${name}`;
            const anime = {
                name,
                searchKey
            }
            axios.post(url, anime)
                .then(res => {
                    toast.success("anime is created!");
                    console.log(res.data);
                })
                .catch(err => {
                    console.log(err);
                    toast.error(err);
                });
        }
        setName("");
        setSearchKey("");
        handleClose();
    }

    const handleCancel = () => {
        setName("");
        setSearchKey("");
        handleClose();
    }

    return (
        <Modal show={show} onHide={handleClose}>
            <Modal.Header closeButton>
                <Modal.Title>Create new anime search</Modal.Title>
            </Modal.Header>
            <Modal.Body>
                <form>
                    <div className="row mb-3">
                        <label htmlFor="name" className="col-sm-3 col-form-label">Name</label>
                        <div className="col-sm-9">
                            <input value={name}
                                onChange={e => setName(e.target.value)}
                                type="text" className="form-control" id="name" />
                        </div>
                    </div>
                    <div className="row mb-3">
                        <label htmlFor="searchKey" className="col-sm-3 col-form-label">Search key</label>
                        <div className="col-sm-9">
                            <input value={searchKey}
                                onChange={e => setSearchKey(e.target.value)}
                                type="text" className="form-control" id="searchKey" />
                        </div>
                    </div>
                </form>
            </Modal.Body>
            <Modal.Footer>
                <Button variant="secondary" onClick={handleCancel}>
                    Close
          </Button>
                <Button variant="primary" onClick={handleSubmit}>
                    Save Changes
          </Button>
            </Modal.Footer>
        </Modal>
    )
}
