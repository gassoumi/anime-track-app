import React from 'react';
import axios from 'axios';
import { toast } from 'react-toastify';
import { Modal, Button } from 'react-bootstrap'

const ApiDeleteAnime = "/api/anime";
export default function ModalDeleteConfirmation({ anime, handleClose, show }) {

    console.log(anime);

    const handleDelete = () => {
        const deleteAnime = async (id) => {
            try {
                const resp = await axios.delete(`${ApiDeleteAnime}/${id}`);
                toast.success("anime is deleted");
            } catch (error) {
                toast.error(error.message);
            } finally {
                handleClose();
            }
        }
        if (anime.name) {
            deleteAnime(anime.name);
        }
    }

    return (
        <>
            <Modal show={show} onHide={handleClose}>
                <Modal.Header closeButton>
                    <Modal.Title>Delete confirmation</Modal.Title>
                </Modal.Header>
                <Modal.Body> Are sure to delete this anime {anime.name} </Modal.Body>
                <Modal.Footer>
                    <Button variant="secondary" onClick={handleClose}>
                        Close
            </Button>
                    <Button variant="primary" onClick={handleDelete}>
                        Delete
            </Button>
                </Modal.Footer>
            </Modal>
        </>
    )
}
